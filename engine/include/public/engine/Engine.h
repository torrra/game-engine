#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/utility/Timer.h"
#include "engine/thread/ThreadManager.h"
#include "engine/Window.h"
#include "engine/core/SceneGraph.h"

#include <string>

namespace engine
{
	class Engine
	{
	public:
		ENGINE_API			Engine(void);
		ENGINE_API			~Engine(void) = default;

		ENGINE_API int16	Startup(const char* projectName, const char* projectDir, uint32 threadCount = DEFAULT_NUM_THREADS);
		ENGINE_API void		ShutDown(void);
		ENGINE_API void		Update(void);
				   void		FixedUpdate(void);

		ENGINE_API Window*	GetWindow(void) const noexcept;

	private:
		inline int16		InitScriptSystem(const char* projectDir);
		inline int16		InitWindow(const char* projectName);
		inline int16		LoadEngineResources(void);

		std::string		m_projectDir;
		Window*			m_window;
		SceneGraph*		m_graph;
		Time			m_time;
		f32				m_timeScale;
	};
}