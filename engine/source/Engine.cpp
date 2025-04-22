#include "Engine.h"

#include "input/Input.h"
#include "resource/ResourceManager.h"
#include "core/systems/ScriptSystem.h"
#include "Window.h"
#include "ui/UIWindow.h"
#include "ui/Application.h"

#include <math/Vector2.hpp>

#include <glad/glad.h>	
#include <iostream>


#undef new
#include <imgui/imgui.h>


#define FIX_UPDATE_FREQUENCY 20
#define TO_MILLISECONDS 0.001f

bool engine::Engine::m_hasEditor = false;


engine::Engine::Engine(bool withEditor)
    : m_projectDir("")
{
    if (!withEditor)
        m_application = new Application();

   m_hasEditor = withEditor;
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

    m_application->Render(m_activeScene.GetGraph());
    m_uiManager.UpdateUI();

    // swaps buffers 
    m_application->EndFrame();
    Input::ResetKeys();
}

bool engine::Engine::HasEditor(void)
{
    return m_hasEditor;
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

inline int16 engine::Engine::InitWindow(const char* /*projectName*/)
{
//    if (Window::StartUp())
//        return ERROR;
//
//    m_window = new Window((projectName) ? projectName : DEFAULT_NAME);
//    // TODO: change dimensions however keep for debugging
//    if (m_window->CreateWindow(1920, 1080))
//        return ERROR;

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

void engine::Engine::FreezeUI(void)
{
    m_frozenUI = true;
}

inline void engine::Engine::UnfreezeUI(void)
{
    m_frozenUI = false;
}
