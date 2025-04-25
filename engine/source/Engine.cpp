#include "Engine.h"

#include "ConsoleLog.hpp"
#include "core/systems/ScriptSystem.h"
#include "input/Input.h"
#include "resource/ResourceManager.h"
#include "serialization/TextSerializer.h"
#include "ui/UIWindow.h"
#include "ui/Application.h"
#include "Window.h"
#include "physics/PhysicsEngine.h"

#include "utility/Platform.h"
#include <iostream>
#include <chrono>

engine::Engine* engine::g_defaultEngine;

engine::Engine::Engine(bool withEditor)
    : m_projectDir("")
{
    if (!withEditor)
        m_application = new Application();

   m_hasEditor = withEditor;

   if (!g_defaultEngine)
       g_defaultEngine = this;
}

int16 engine::Engine::Startup(const char* projectName, const char* projectDir, uint32 threadCount)
{
    if (!projectDir)
        std::printf("No project opened\n");

    m_application->Startup(projectName);

    ThreadManager::Startup(threadCount);

    if (InitScriptSystem(projectDir) != SUCCESS)
        return ERROR;

    if (Input::StartUp() != SUCCESS)
        return ERROR;

    Input::SetCursorMode(ECursorMode::MODE_NORMAL);

    // Load default resources
    if (LoadEngineResources() != SUCCESS)
        return ERROR;

    
    m_uiManager = UIManager(m_application->GetWindow()->GetPtr());
    PhysicsEngine::Get().Init();
    return SUCCESS;
}

int16 engine::Engine::Startup(uint32 threadCount)
{
    if (!m_hasEditor)
        m_application->Startup(m_currentProject.m_executableName.c_str());

    ThreadManager::Startup(threadCount);

    ScriptSystem::SetUserScriptLocation(m_projectDir.string().c_str());
    ScriptSystem::Startup();
    PhysicsEngine::Get().Init();

    if (Input::StartUp() != SUCCESS)
        return ERROR;

    Input::SetCursorMode(ECursorMode::MODE_NORMAL);

    // Load default resources
    if (LoadEngineResources() != SUCCESS)
        return ERROR;

    m_uiManager = UIManager(m_application->GetWindow()->GetPtr());
    std::filesystem::path scenePath = m_projectDir;

    if (m_hasEditor)
        scenePath.append(m_currentProject.m_defaultEditorScene);
    else
        scenePath.append(m_currentProject.m_defaultGameScene);

    m_activeScene.LoadNewScene(false, scenePath);
    return SUCCESS;
}


void engine::Engine::ShutDown(void)
{
    ThreadManager::Shutdown();
    ScriptSystem::Shutdown();
    ResourceManager::ShutDown();
    m_application->Shutdown();
    Input::ShutDown();
    m_uiManager.ShutDown();
    PhysicsEngine::Get().CleanUp();

    if (m_application)
        delete m_application;
}


void engine::Engine::SetProject(const char* projectDir)
{
    InitScriptSystem(projectDir);
}

void engine::Engine::SetEditorApplication(Application* ptr)
{
    m_application = ptr;
    ptr->SetCurrentScene(&m_activeScene);
}

engine::Window* engine::Engine::GetWindow(void) const noexcept
{
    return m_application->GetWindow();
}

engine::SceneGraph* engine::Engine::GetGraph(void)
{
    return m_activeScene.GetGraph();
}

void engine::Engine::UpdateGameplay(void)
{
    m_activeScene.Tick();
}

void engine::Engine::UpdateApplicationWindow(void)
{
    m_application->BeginFrame();
    m_uiManager.NewFrame();

    m_application->Render(m_activeScene.GetGraph());

    m_uiManager.EndFrame();
    m_application->EndFrame();
    Input::ResetKeys();
}

bool engine::Engine::HasEditor(void)
{
    return g_defaultEngine->m_hasEditor;
}


const std::filesystem::path& engine::Engine::GetProjectDir(void) const
{
    return m_projectDir;
}

void engine::Engine::LoadNewScene(bool serialize, const std::filesystem::path& path)
{
    ThreadManager::AddTask([this, path, serialize]
        {
            m_activeScene.LoadNewScene(serialize, path);
            ThreadManager::AddTask<ThreadManager::ETaskType::GRAPHICS>(
                &Application::SetCurrentScene, m_application, &m_activeScene);
        });
}

void engine::Engine::CreateProject(const std::string & dir, const std::string& name)
{
    if (name.length() > 64)
    {
        PrintLog(ErrorPreset(), "Unable to create project: name is too long! (over 64 characters)");
        return;
    }
    std::filesystem::path pathObj = dir;

    if (!pathObj.is_absolute())
        pathObj = std::filesystem::absolute(pathObj);

    if (!std::filesystem::exists(pathObj))
    {
        PrintLog(ErrorPreset(), "Unable to create project: target location\
 does not exist or path is not absolute.");
        return;
    }

    pathObj.append(name);

    if (std::filesystem::exists(pathObj))
    {
        PrintLog(WarningPreset(), "Unable to create project: directory already \
exists. Attempting to open project file...");

        std::filesystem::path filename = pathObj.filename().replace_extension(".mustang");
        pathObj += std::filesystem::path::preferred_separator;

        OpenProject(pathObj += filename);
        return;
    }

    std::filesystem::create_directory(pathObj);
    pathObj.append(name);
    pathObj.replace_extension(".mustang");

    m_projectFile = pathObj;
    m_currentProject.m_defaultEditorScene =  m_currentProject.m_defaultGameScene = "default.mscn";
    m_currentProject.m_executableName = name;

    m_projectDir = pathObj.remove_filename().append("assets");
    std::filesystem::create_directory(pathObj);

    m_activeScene.EditPath(m_projectDir);
    m_activeScene.Rename("default");
    SaveProject();
}

void engine::Engine::OpenProject(const std::filesystem::path& projFile)
{
    constexpr wchar_t extension[] = L".mustang";
    
    m_projectFile = projFile;

    if (memcmp(projFile.extension().c_str(), extension, sizeof(extension)) != 0)
    {
        PrintLog(ErrorPreset(), "Invalid project file");
        return;
    }

    std::ifstream input(projFile, std::ios::in | std::ios::binary);

    if (!input)
    {
        PrintLog(ErrorPreset(), "Unable to open project file");
        return;
    }

    const char* cursor;
    const char* end;
    const char* fileData = text::LoadFileData(input, cursor, end);

    m_projectDir = projFile.parent_path();
    m_projectDir.append("assets");

    DeserializeProjectFile(cursor, end);
    text::UnloadFileData(fileData);
}

void engine::Engine::SaveProject(void)
{
    std::ofstream output(m_projectFile, std::ios::out);

    text::Serialize(output, "defaultGameScene", m_currentProject.m_defaultGameScene);
    output << '\n';
    text::Serialize(output, "defaultEditorScene", m_currentProject.m_defaultEditorScene);
    output << '\n';
    text::Serialize(output, "executableName", m_currentProject.m_executableName);

    m_activeScene.SerializeText();
}

inline int16 engine::Engine::InitScriptSystem(const char* projectDir)
{
    static bool initialized = false;

    ScriptSystem::SetCurrentScene(m_activeScene.GetGraph());

    if (!initialized)
    {
        ScriptSystem::Startup();
        initialized = true;
    }
    /*
    * TODO: probably dont need this line, as you should not be able to 
    *		open script files if no projects were selected. 
    */
    std::string path((projectDir) ? projectDir : ".\\");

    ScriptSystem::SetUserScriptLocation(path.c_str());
    ScriptSystem::RunAllUserScripts();
    
    return SUCCESS;
}


inline int16 engine::Engine::LoadEngineResources(void)
{
    ResourceManager::LoadShader(
        "Default", 
        ".\\shaders\\Default.vs", 
        ".\\shaders\\Default.frag"
    );

    return SUCCESS;
}


engine::GameScene& engine::Engine::GetCurrentScene(void)
{
    return m_activeScene;
}


void engine::Engine::SetExecutableName(const std::string& name)
{
    m_currentProject.m_executableName = name;
    SaveProject();
}

void engine::Engine::SetDefaultGameScene(const std::string& relativePath)
{
    std::filesystem::path sceneFile = m_projectDir;

    sceneFile.append(relativePath);

    if (!std::filesystem::exists(sceneFile))
    {
        PrintLog(ErrorPreset(), "Invalid scene file");
        return;
    }

    m_currentProject.m_defaultGameScene = relativePath;
    SaveProject();
}

void engine::Engine::SetDefaultEditorScene(const std::string& relativePath)
{
    std::filesystem::path sceneFile = m_projectDir;

    sceneFile.append(relativePath);

    if (!std::filesystem::exists(sceneFile))
    {
        PrintLog(ErrorPreset(), "Invalid scene file");
        return;
    }

    m_currentProject.m_defaultEditorScene = relativePath;
    SaveProject();
}


void engine::Engine::DeserializeProjectFile(const char* cursor, const char* end)
{
    cursor = text::DeserializeString(cursor, end, m_currentProject.m_defaultGameScene);
    if (memcmp(m_currentProject.m_defaultGameScene.c_str(), "0", 1) == 0)
        PrintLog(WarningPreset(), "No default scene set for standalone mode");

    cursor = text::DeserializeString(cursor, end, m_currentProject.m_defaultEditorScene);
    cursor = text::DeserializeString(cursor, end, m_currentProject.m_executableName);

    if (m_hasEditor)
    {
        if (memcmp(m_currentProject.m_defaultEditorScene.c_str(), "0", 1) == 0)
            PrintLog(WarningPreset(), "No default scene set for editor mode");

        PrintLog(InfoPreset(), "Executable name: " + m_currentProject.m_executableName);
    }
}

void engine::Engine::BuildProjectExecutable(const std::filesystem::path& destination)
{
    using TimePoint = std::chrono::system_clock::time_point;

    std::filesystem::path buildDir = destination;

    TimePoint now = std::chrono::system_clock::now();
    std::time_t timeVal = std::chrono::system_clock::to_time_t(now);

    buildDir.append(m_currentProject.m_executableName + +"-" +
    std::to_string(math::Absolute(SceneGraph::RandomNumber())) + "-" + std::to_string(timeVal));

    constexpr uint32 loopSafety = 100;
    uint32 attempt = 0;

    while (std::filesystem::exists(buildDir) && attempt++ < loopSafety)
    {
        PrintLog(WarningPreset(), "An entry with the executable's name already exists. Re-generating build name");

        if (attempt != loopSafety)
        {
            buildDir = destination;
            buildDir.append(m_currentProject.m_executableName + +"-" +
            std::to_string(math::Absolute(SceneGraph::RandomNumber())) + "-" + std::to_string(timeVal));
        }
        else
        {
            PrintLog(ErrorPreset(), "Unable to create build folder");
            return;
        }
    }

    std::filesystem::copy_options copyOptions = std::filesystem::copy_options::skip_existing |
                                                std::filesystem::copy_options::recursive;

    std::filesystem::create_directory(buildDir);

    buildDir.append(m_currentProject.m_executableName).replace_extension(".mustang");
    std::filesystem::copy_file(m_projectFile, buildDir);

    buildDir.remove_filename().append("assets");
    std::filesystem::copy(m_projectDir, buildDir, copyOptions);

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::copy(currentPath, buildDir.parent_path(), copyOptions);

    std::filesystem::path exePath = currentPath = buildDir.replace_filename("defaultExecutable.exe");
    std::filesystem::rename(currentPath,
    exePath.replace_filename(m_currentProject.m_executableName).replace_extension(".exe"));

    // Directly execute .exe
    OpenFile(std::filesystem::absolute(exePath).c_str(),
             std::filesystem::absolute(buildDir.parent_path()).c_str());

}
