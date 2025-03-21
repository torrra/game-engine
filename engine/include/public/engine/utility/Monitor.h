#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>

namespace engine
{
	class Monitor
	{
	public:
		ENGINE_API static math::Vector2i GetPrimaryMonitorSize(void);
	};
}