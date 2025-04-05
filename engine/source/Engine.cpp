#include "Engine.h"

#include "input/Input.h"
#include "resource/ResourceManager.h"
#include "core/systems/ScriptSystem.h"
#include "Window.h"
#include "sceneGraphViewer/SceneGraphUI.h"
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

    Input::SetCursorMode(ECursorMode::NORMAL);

    m_uiManager = UIManager(m_window->GetPtr());
    m_viewport = new Viewport("Viewport");


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

    if (m_graphViewer)
        delete m_graphViewer;

    if (m_viewport)
        delete m_viewport;

    if (m_window)
        delete m_window;
    
    if (m_graph)
        delete m_graph;
}

void engine::Engine::Update(void)
{	
    static bool isInit = false;

    if (!isInit)
    {
        m_graphViewer = new SceneGraphViewer("Scene Graph", m_graph);
        isInit = true;
    }

    static const f32 updateFrequency = (FIX_UPDATE_FREQUENCY * m_timeScale) * TO_MILLISECONDS;
    static f32 runTime = 0.0f;
    static f32 nextUpdateTime = runTime + updateFrequency;

    // Sync game logic
    ThreadManager::SynchronizeGameThread(m_graph);
    
    // Update physics (fixed update)
    runTime = m_time.GetTotalTime();
    if (runTime >= nextUpdateTime)
    {
        nextUpdateTime = runTime + updateFrequency;

        FixedUpdate();
    }

    // Update game logic
    ThreadManager::UpdateGameLogic(m_graph, m_time.GetDeltaTime());
    
    m_window->ClearWindow(0.1f, 0.1f, 0.1f);
    
    m_uiManager.NewFrame();
    m_graphViewer->Render();
    m_viewport->DrawViewport(); // DRAW VIEWPORT
    m_uiManager.UpdateUI();
    // Render
    m_viewport->RenderToViewport(m_graph);
    Input::ResetKeys();
    m_window->Update();
    m_time.Update();
}

void engine::Engine::RunTimeUpdate(void)
{
    // TODO: add run time update logic
}

void engine::Engine::FixedUpdate(void)
{
    // TODO: add fixed update logic
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
    if (m_window->CreateWindow(800, 600))
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
