#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

namespace engine
{
	class Time
	{
	public:
		ENGINE_API Time(void);
		ENGINE_API ~Time(void) = default;

		ENGINE_API void Update();

		ENGINE_API f32 GetDeltaTime(void);
		ENGINE_API f32 GetTotalTime(void);
	
	private:
		f32 m_deltaTime;
		f32 m_totalTime;
		f32 m_lastTime;
	};

	extern Time g_engineTime;
}