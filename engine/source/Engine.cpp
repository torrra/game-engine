#include "Engine.h"

#include "input/Input.h"
#include "resource/ResourceManager.h"
#include "core/systems/ScriptSystem.h"
#include "utility/Monitor.h"
#include "Window.h"

#include <math/Vector2.hpp>

#define SUCCESS		0
#define ERROR		1

#define DEFAULT_NAME "Editor";

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

	if (InitScriptSystem(projectDir))
		return ERROR;

	if (InitWindow(projectName))
		return ERROR;

	// TODO: init UI manager

	
	// Initialize engine time
	m_time = Time();

	// Load default resources
	if (LoadEngineResources())
		return ERROR;

	return SUCCESS;
}

void engine::Engine::ShutDown(void)
{
	ThreadManager::Shutdown();
	ScriptSystem::Shutdown();
	ResourceManager::ShutDown();
	Input::ShutDown();

	// TODO: shut down ui manager

	if (m_window)
	{
		m_window->Shutdown();
		delete m_window;
	}
	
	if (m_graph)
		delete m_graph;
}

void engine::Engine::Update(void)
{	
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

	ThreadManager::RenderScene(m_graph);

	// Render
	Input::ResetKeys();
	m_window->UpdateBuffers();
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

	std::string path((projectDir) ? projectDir : "..\\");

	ScriptSystem::SetUserScriptLocation(path.c_str());
	ScriptSystem::RunAllUserScripts();
	
	return SUCCESS;
}

inline int16 engine::Engine::InitWindow(const char* projectName)
{
	std::string windowTitle(projectName);

	if (!projectName)
	{
		windowTitle = DEFAULT_NAME;
	}

	if (Window::InitGLFW())
		return ERROR;

	// By default open the editor full screen on primary monitor
	math::Vector2i monitorSize = Monitor::GetPrimaryMonitorSize();
	m_window = new Window(windowTitle.c_str(), monitorSize.GetX(), monitorSize.GetY(), true);

	return (m_window->GetWindowPtr()) ? SUCCESS : ERROR;
}

inline int16 engine::Engine::LoadEngineResources(void)
{
	ResourceManager::LoadShader(
		"Default", 
		"..\\engineShader\\Default.vs", 
		"..\\engineShader\\Default.frag"
	);

	return SUCCESS;
}
