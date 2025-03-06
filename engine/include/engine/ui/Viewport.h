#pragma once

#include "engine/EngineExport.h"

#include <math/Vector2.hpp>

namespace engine
{
	class Viewport
	{
	public:
		ENGINE_API Viewport(void) = default;
		ENGINE_API ~Viewport(void) = default;
	private:
		math::Vector2<uint32_t> m_size;

	};
}