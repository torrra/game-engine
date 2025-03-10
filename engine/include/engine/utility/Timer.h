#pragma once

#include "engine/EngineExport.h"

namespace engine
{
	class Time
	{
	public:
		ENGINE_API Time(void);
		ENGINE_API ~Time(void) = default;

		ENGINE_API void Update();

		ENGINE_API float GetDeltaTime(void);
		ENGINE_API float GetTotalTime(void);
	
	private:
		float m_deltaTime;
		float m_totalTime;
		float m_lastTime;
	};

	extern Time g_engineTime;
}