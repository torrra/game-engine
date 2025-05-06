#include "ui/MenuBar.h"
#include "ui/EditorApplication.h"
#include "ui/SceneGraph.h"
#include <engine/Engine.h>
#include <engine/ui/UIComponent.h>
#include <engine/game/GameScene.h>
#include <engine/utility/MemoryCheck.h>

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
        ProjectMenu();
        UpdateStartButton(activeScene);

        ::ui::EndMainMenuBar();
    }
}

void editor::MenuBar::UpdateStartButton(engine::GameScene& activeScene)
{
    if (m_gameRunning && ::ui::Button("Stop"))
    {
        engine::ThreadManager::SynchronizeGameThread(nullptr);
        m_gameRunning = false;
        activeScene.Stop();
        printf("Game running: false\n");

        m_application->m_graphView.ClearGraph();
        activeScene.Reset();
        engine::ThreadManager::SynchronizeGameThread(activeScene.GetGraph());
        m_application->m_graphView.SetGraph(activeScene.GetGraph());
        
        // Re-select the previously selected entity for property window
        m_application->m_properties.SetHandle(m_application->m_graphView.GetSelectedEntity());
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

void editor::MenuBar::ProjectMenu(void)
{
    ui::StartDisabledSection(m_gameRunning);
    if (ui::StartMenu("Project"))
    {
        if (ui::MenuItem("Open project"))
        {
            OpenProject();
        }

        ui::EndMenu();
    }
    ui::EndDisabledSection();

}

void editor::MenuBar::OpenProject(void)
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
                printf("[WARNING]: Failed to select file\n");
            
            pFileOpen->Release();
        }
        else
            printf("[ERROR]: Failed to create windows com instance\n");

        CoUninitialize();
    }
    else
        printf("[ERROR]: Failed to initialize windows com library\n");


    return false;
}
