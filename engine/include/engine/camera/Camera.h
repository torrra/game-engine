#pragma once

#include "math/VectorGeneric.hpp"
#include "math/MatrixGeneric.hpp"

#include "engine/EngineExport.h"

namespace engine
{
	class Camera
	{
	public:
		ENGINE_API 						Camera(void) = default;
		ENGINE_API 						Camera(math::Vector3<float> position, float speed);
		ENGINE_API 						~Camera(void) = default;

		ENGINE_API math::Matrix4<float>	GetPerspectiveMatrix(float near, float far, float fovDeg, float aspect) const noexcept;
		ENGINE_API math::Matrix4<float>	GetViewMatrix(void);
		ENGINE_API math::Vector3<float>	GetPosition(void) const noexcept;
		ENGINE_API math::Vector3<float>& GetPosition(void);
		ENGINE_API void					CameraInput(float deltaTime);
		ENGINE_API void					MouseMotion(math::Vector2<float> const& cursorPos, float deltaTime);

	private:
		// View matrix
		math::Vector3<float>	m_position;
		math::Vector3<float>	m_up;
		math::Vector3<float>	m_right;
		math::Vector3<float>	m_forward;

		// Config
		float					m_speed;
		float					m_angularSpeed;
	};
}
