#include "Engine.h"

#include "input/Input.h"
#include "resource/ResourceManager.h"
#include "core/systems/ScriptSystem.h"
#include "Window.h"
#include "ui/UIWindow.h"

#include <math/Vector2.hpp>

#include <glad/glad.h>	
#include <iostream>


#undef new
#include <imgui/imgui.h>

#define SUCCESS		0
#define ERROR		1

#define DEFAULT_NAME "Editor"

#define FIX_UPDATE_FREQUENCY 20
#define TO_MILLISECONDS 0.001f

engine::Engine::Engine(void)
    : m_graph(nullptr), m_window(nullptr), m_projectDir("\0"), m_timeScale(1.0f)
{
}

int16 engine::Engine::Startup(const char* projectName, const char* projectDir, uint32 threadCount)
{
    if (!projectDir)
        std::printf("No project opened\n");

    ThreadManager::Startup(threadCount);
    
    m_graph = new SceneGraph();

    if (InitScriptSystem(projectDir) != SUCCESS)
        return ERROR;

    if (InitWindow(projectName) != SUCCESS)
        return ERROR;

    if (Input::StartUp() != SUCCESS)
        return ERROR;

    Input::SetCursorMode(ECursorMode::MODE_NORMAL);

    m_uiManager = UIManager(m_window->GetPtr());

    // Initialize engine time
    m_time = Time();

    // Load default resources
    if (LoadEngineResources() != SUCCESS)
        return ERROR;

    
    return SUCCESS;
}

void engine::Engine::ShutDown(void)
{
    ThreadManager::Shutdown();
    ScriptSystem::Shutdown();
    ResourceManager::ShutDown();
    Input::ShutDown();
    Window::ShutDown();

    // TODO: call shutdown for ui manager
    m_uiManager.ShutDown();

    if (m_window)
        delete m_window;
    
    if (m_graph)
        delete m_graph;
}

void engine::Engine::Update(void)
{

    // Sync game logic
    ThreadManager::SynchronizeGameThread(m_graph);
    ThreadManager::UpdateGameLogic(m_graph, m_time.GetDeltaTime());

    m_window->ClearWindow(0.1f, 0.1f, 0.1f);
    ThreadManager::RenderScene(m_graph);

   // m_uiManager.NewFrame();
}

void engine::Engine::PostUpdate(::ui::UIWindow* viewport)
{
    Input::ResetKeys();

    if (viewport)
        viewport->Render();

    //m_uiManager.UpdateUI();
    m_window->Update();
    m_time.Update();
}

void engine::Engine::RunTimeUpdate(void)
{
}

void engine::Engine::FixedUpdate(void)
{
    // TODO: add fixed update logic
}

void engine::Engine::SetGameState(bool run)
{
    m_gameState = run;

    if (run)
        ThreadManager::AddTask(&Engine::RunGame, this);
}

void engine::Engine::SetProject(const char* projectDir)
{
    InitScriptSystem(projectDir);
}

engine::Window* engine::Engine::GetWindow(void) const noexcept
{
    return m_window;
}


engine::Time& engine::Engine::GetTime(void) noexcept
{
    return m_time;
}

engine::SceneGraph* engine::Engine::GetGraph(void)
{
    return m_graph;
}

engine::UIManager engine::Engine::GetUI(void) const noexcept
{
    return m_uiManager;
}

void engine::Engine::RunGame()
{
    printf("Running...\n");

    // Update game logic
    while (m_gameState)
    {
    
    }

}

inline int16 engine::Engine::InitScriptSystem(const char* projectDir)
{
    static bool initialized = false;

    ScriptSystem::SetCurrentScene(m_graph);

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

inline int16 engine::Engine::InitWindow(const char* projectName)
{
    if (Window::StartUp())
        return ERROR;

    m_window = new Window((projectName) ? projectName : DEFAULT_NAME);
    // TODO: change dimensions however keep for debugging
    if (m_window->CreateWindow(1920, 1080))
        return ERROR;

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
