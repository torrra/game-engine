#include "Engine.h"

#include "input/Input.h"
#include "resource/ResourceManager.h"
#include "core/systems/ScriptSystem.h"
#include "utility/Monitor.h"
#include "Window.h"

#include <math/Vector2.hpp>

#define USER_SCRIPT_PATH "\\assets\\scripts\\"

#define ERROR -1
#define SUCCESS 0

#define FIX_UPDATE_PER_SECOND 20
#define TO_MILLISECONDS 0.001f

engine::Engine::Engine(void)
	: m_graph(nullptr), m_window(nullptr), m_timeScale(1.0f)
{
}

int16 engine::Engine::Startup(const char* projectName, const char* projectDir, uint32 threadCount)
{
	int16 result = SUCCESS;

	ThreadManager::Startup(threadCount);
	
	m_graph = new SceneGraph();

	if (InitScriptSystem(projectDir)) 
		return result;

	if (InitWindow(projectName))
		return result;

	// TODO: init UI manager

	
	// Initialize engine time
	m_time = Time();

	// Load default resources
	LoadEngineResources();

	return result;
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
	ThreadManager::SynchronizeGameThread(m_graph);
	ThreadManager::UpdateGameLogic(m_graph, m_time.GetDeltaTime());

	m_window->ClearWindow();

	m_window->UpdateBuffers();
	m_time.Update();
}

void engine::Engine::FixedUpdate(void)
{
	static const f32 updateFrequency = (FIX_UPDATE_PER_SECOND * m_timeScale) * TO_MILLISECONDS;
	
	f32 runTime = 0.0f;
	f32 nextUpdateTime = runTime + updateFrequency;

	while (!m_window->ShouldWindowClose())
	{
		runTime = m_time.GetTotalTime();
		if (runTime >= nextUpdateTime)
		{
			nextUpdateTime = runTime + updateFrequency;

#ifdef DEBUG_FIXED_UPDATE
			static f32 prevTime = m_time.GetTotalTime();
			std::printf("Update delta time %.5fs\n", runTime - prevTime);
			prevTime = runTime;
#endif
			// Place functions needing fixed update here...

		}

		m_time.Update();
	}
}

engine::Window* engine::Engine::GetWindow(void) const noexcept
{
	return m_window;
}

inline int16 engine::Engine::InitScriptSystem(const char* projectDir)
{
	char scriptPath[256];
	memcpy(scriptPath, projectDir, strlen(projectDir) + strlen(USER_SCRIPT_PATH));
	strcat_s(scriptPath, USER_SCRIPT_PATH);

	ScriptSystem::SetUserScriptLocation(scriptPath);
	ScriptSystem::Startup(); // TODO: Check for successful init

	return SUCCESS;
}

inline int16 engine::Engine::InitWindow(const char* projectName)
{
	if (Window::InitGLFW())
		return ERROR;

	// By default open the editor full screen on primary monitor
	math::Vector2i monitorSize = Monitor::GetPrimaryMonitorSize();
	m_window = new Window(projectName, monitorSize.GetX(), monitorSize.GetY(), true);

	return (m_window->GetWindowPtr()) ? SUCCESS : ERROR;
}

inline int16 engine::Engine::LoadEngineResources(void)
{
	// TODO: Load engine shaders, scripts, fonts, etc...
	ResourceManager::LoadShader(
		"Default", 
		"..\\engineShader\\Default.vs", 
		"..\\engineShader\\Default.frag"
	);

	return SUCCESS;
}
