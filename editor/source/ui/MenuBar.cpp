#include "ui/MenuBar.h"
#include "ui/EditorApplication.h"
#include "ui/SceneGraph.h"
#include <engine/Engine.h>
#include <engine/ui/UIComponent.h>
#include <engine/ui/UITable.h>
#include <engine/game/GameScene.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/ConsoleLog.hpp>
#include <engine/input/InputHandler.h>
#include <engine/ui/IconsLucide.h>

#define CREATE_PROJECT_IN_PROGRESS 0
#define CREATE_PROJECT_SUCCESS 1
#define CREATE_PROJECT_FAILED 2

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShlObj_core.h>

editor::MenuBar::MenuBar(EditorApplication* application)
    : m_application(application)
{
}

void editor::MenuBar::Render(engine::GameScene& activeScene)
{
    if (::ui::StartMainMenuBar())
    {
        DisplayCurrentProject();
        ProjectMenu(activeScene);
        UpdateStartButton(activeScene);
        CloseProgram();

        ::ui::EndMainMenuBar();
    }

    if (m_isCreateWndOpened)
    {
        ui::OpenModal("Create Project");
        CreateProject();
    }
    
}

void editor::MenuBar::UpdateStartButton(engine::GameScene& activeScene)
{
    if (m_gameRunning && ::ui::Button("Stop"))
    {
        m_gameRunning = false;
        m_application->ResetScene(activeScene);
        
        
        // Re-select the previously selected entity for property window
        m_application->m_properties.SetHandle(m_application->m_graphView.GetSelectedEntity());

        printf("Game running: false\n");
    }
    else if (!m_gameRunning && ::ui::Button("Start"))
    {
        activeScene.Start();
        m_gameRunning = true;

        printf("Game running: true\n");
    }
}

void editor::MenuBar::DisplayCurrentProject(void)
{
    engine::Engine* engine = engine::Engine::GetEngine();


    ui::Text(engine->GetProjectName().c_str());
}

void editor::MenuBar::ProjectMenu(::engine::GameScene& scene)
{
    ui::StartDisabledSection(m_gameRunning);
    
    if (ui::StartMenu("Project"))
    {
        if (ui::MenuItem("Save project", "Ctrl+S"))
        {
            engine::Engine::GetEngine()->SaveProject();
        }

        if (ui::MenuItem("Create project"))
        {
            m_isCreateWndOpened = true;
        }

        if (ui::MenuItem("Open project"))
        {
            OpenProject(scene);
        }

        if (ui::MenuItem("Build project"))
        {
            std::filesystem::path buildPath;
            SelectFolder(buildPath);

            if (!buildPath.empty())
                engine::Engine::GetEngine()->BuildProjectExecutable(buildPath);
           
        }

        ui::EndMenu();
    }
    ui::EndDisabledSection();
}

void editor::MenuBar::CloseProgram(void)
{
    f32 width = ui::GetPos().GetX() + ui::GetAvailSpace().GetX();

    ui::SameLine(width - 30.0f);
    if (ui::Button(ICON_LC_X))
        m_application->GetWindow()->CloseWindow();
}

void editor::MenuBar::OpenProject(::engine::GameScene& scene)
{
    std::filesystem::path projectPath;

    if (SelectProject(projectPath))
    {
        engine::Engine* engine = engine::Engine::GetEngine();
        if (engine)
        {
            printf("Opening project: %s\n", projectPath.string().c_str());
            
            // Close project
            engine::ThreadManager::SynchronizeGameThread(engine->GetCurrentScene().GetGraph());
            engine->SaveProject();

            // Clean editor prior to opening new project
            m_application->m_properties.SetHandle(engine::Entity::EHandleUtils::INVALID_HANDLE);
            m_application->m_graphView.ClearGraph();

            // Open new project
            engine->OpenProject(projectPath);
            engine->LoadDefaultScene();

            m_application->m_graphView.SetGraph(scene.GetGraph());
            m_application->m_assetWnd.SetPath(engine->GetProjectDir());
            m_application->m_sceneEditorView->GetPicking()->InitEntities(engine->GetGraph());
        }
    }
}

bool editor::MenuBar::SelectProject(std::filesystem::path& projectPath)
{
    HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    // Check com library initialized successfully
    if (SUCCEEDED(result))
    {
        IFileOpenDialog* pFileOpen;
        result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
        pFileOpen->SetOptions(FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM);

        // Check instance created successfully
        if (result == S_OK)
        {
            // Display windows file selection modal
            result = pFileOpen->Show(NULL);

            // Successfully selected file
            if (result == S_OK)
            {
                LPWSTR filePath;
                IShellItem* pItem;
                result = pFileOpen->GetResult(&pItem);
                pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

                // Set new project directory
                projectPath = filePath;

                if (SUCCEEDED(result))
                    CoTaskMemFree(filePath);
                
                pItem->Release();
                pFileOpen->Release();
                CoUninitialize();

                return true;
            }
            else
                engine::PrintLog(engine::WarningPreset(), "Failed to select file\n");
            
            pFileOpen->Release();
        }
        else
            engine::PrintLog(engine::ErrorPreset(), "Failed to create windows com instance\n");

        CoUninitialize();
    }
    else
        engine::PrintLog(engine::ErrorPreset(), "Failed to initialize windows com library\n");


    return false;
}

void editor::MenuBar::CreateProject(void)
{
    // Open create project modal
    if (ui::StartModal("Create Project", m_isCreateWndOpened))
    {
        static std::string projectName;
        static std::filesystem::path projectPath;
        static ui::Table table("modalLayout", 3, ui::GetAvailSpace());
        static uint8 createProjectStatus = CREATE_PROJECT_IN_PROGRESS;

        if (engine::InputHandler::IsInputPressed(KEY_ESCAPE))
        {
            // Explicitly call destructor to prevent memory leaks
            table.~Table();
            CloseCreateMenu(projectName, projectPath, createProjectStatus);
        }

        // Use table for alignment
        if (table.StartTable())
        {
            // Line 1 - project name
            SetProjectName(table, projectName);

            // Line 2 - project path
            SelectProjectPath(table, projectPath);

            // Line 3 - add Create project button + success info
            if (ui::Button("Create Project"))
                createProjectStatus = ((!projectName.empty() || !projectPath.empty()) && projectName.size() < 64) ? CREATE_PROJECT_SUCCESS : CREATE_PROJECT_FAILED;
            
            // Project creation status (display either 'success' or 'failed' text in menu)
            ui::SameLine();
            if (createProjectStatus)
            {
                ui::SameLine(160.f);
                if (createProjectStatus == CREATE_PROJECT_SUCCESS)
                {
                    ui::Text("Success");
                    
                    // Create & open project
                    engine::Engine* engine = engine::Engine::GetEngine();
                    if (engine)
                    {
                        // Close project
                        engine::ThreadManager::ExecuteRenderThreadTasks();
                        engine->SaveProject();

                        // Clean editor prior to opening new project
                        m_application->m_properties.SetHandle(engine::Entity::EHandleUtils::INVALID_HANDLE);
                        m_application->m_graphView.ClearGraph();
                        engine->GetCurrentScene().Reset(false);

                        if (engine->CreateProject(projectPath.string(), projectName))
                        {
                            // Open new project
                            projectPath.append(projectName)
                                       .append(projectName)
                                       .replace_extension(".mustang");

                            engine->OpenProject(projectPath);
                            engine->LoadDefaultScene();

                            m_application->m_assetWnd.SetPath(engine->GetProjectDir());
                        }
                    }
                    
                    // Explicitly call destructor to prevent memory leaks
                    table.~Table();
                    CloseCreateMenu(projectName, projectPath, createProjectStatus);
                }
                else
                    ui::Text("Failed");
            }
        }

        ui::EndPopUp();
    }
}

bool editor::MenuBar::SelectFolder(std::filesystem::path& projectPath)
{
    HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    // Check com library initialized successfully
    if (SUCCEEDED(result))
    {
        IFileOpenDialog* pFolderOpen;
        result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFolderOpen));
        pFolderOpen->SetOptions(FOS_PICKFOLDERS | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM);

        // Check instance created successfully
        if (result == S_OK)
        {
            // Display windows folder selection modal
            result = pFolderOpen->Show(NULL);

            // Successfully selected file
            if (result == S_OK)
            {
                LPWSTR path;
                IShellItem* pItem;
                result = pFolderOpen->GetResult(&pItem);
                pItem->GetDisplayName(SIGDN_FILESYSPATH, &path);

                // Set new project directory
                projectPath = path;

                if (SUCCEEDED(result))
                    CoTaskMemFree(path);

                pItem->Release();
                pFolderOpen->Release();
                CoUninitialize();

                return true;
            }
            else
                engine::PrintLog(engine::WarningPreset(), "Failed to select folder\n");

            pFolderOpen->Release();
        }
        else
            engine::PrintLog(engine::ErrorPreset(), "Failed to create windows com instance\n");

        CoUninitialize();
    }
    else
        engine::PrintLog(engine::ErrorPreset(), "Failed to initialize windows com library\n");


    return false;
}

void editor::MenuBar::SetProjectName(ui::Table& contentTable, std::string& name)
{
    ui::VerticalSpacing();

    // Display text + format layout
    contentTable.NextRow();
    contentTable.NextColumn();
    ui::Text("Project name: ");
    ui::VerticalSpacing();

    // Add interactable input field for user to write the project name + format layout 
    contentTable.NextColumn(500.0f);
    ui::ItemWidth(150.0f);
    ui::InputBox("##ProjectName", "Project name", name, false);
    ui::VerticalSpacing();

    // Display current name
    contentTable.NextColumn();
    ui::Text(name.c_str());
    ui::VerticalSpacing();
}

void editor::MenuBar::SelectProjectPath(ui::Table& contentTable, std::filesystem::path& path)
{
    contentTable.NextRow();
 
    // Display text
    contentTable.NextColumn();
    ui::Text("Project path: ");
    ui::VerticalSpacing();

    // Interactable button to open file browser
    contentTable.NextColumn();
    if (ui::Button("Select"))
    {
        SelectFolder(path);
        printf("path: %s\n", path.string().c_str());
    }
    ui::VerticalSpacing();

    /*
        Display current parent directory (we could display the entire absolute
        path however this risks to destroy formatting).
    */
    contentTable.NextColumn();
    ui::Text(path.filename().string().c_str());
    ui::VerticalSpacing();
    contentTable.EndTable();
}

void editor::MenuBar::CloseCreateMenu(std::string& projectName, std::filesystem::path& projectPath, uint8& status)
{
    projectName.clear();
    projectPath.clear();
    
    projectName.~basic_string();
    projectPath.~path();

    m_isCreateWndOpened = false;
    status = CREATE_PROJECT_IN_PROGRESS;


}
