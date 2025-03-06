#pragma once

#include <math/Vector2.hpp>
#include <math/Vector3.hpp>

namespace engine
{
	struct Vertex
	{
		math::Vector3<float>	m_position;
		math::Vector3<float>	m_normal;
		math::Vector3<float>	m_tangent;
		math::Vector3<float>	m_biTangent;
		math::Vector2<float>	m_texCoord;
	};
}