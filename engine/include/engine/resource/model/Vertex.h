#pragma once

#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>
#include <math/Vector3.hpp>

namespace engine
{
	struct Vertex
	{
		math::Vector3f	m_position;
		math::Vector3f	m_normal;
		math::Vector3f	m_tangent;
		math::Vector3f	m_biTangent;
		math::Vector2f	m_texCoord;
	};
}