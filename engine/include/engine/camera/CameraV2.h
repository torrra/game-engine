#pragma once

#include <math/Vector3.hpp>
#include <math/Matrix4.hpp>

#include "engine/EngineExport.h"


namespace engine
{
	struct Frustum
	{
		float m_near;
		float m_far;
		float m_fovRad;
		float m_ratio;
		
		ENGINE_API Frustum(float near, float far, float fovDeg, float ratio)
			: m_near(near), m_far(far), m_fovRad(fovDeg * DEG2RAD), m_ratio(ratio)
		{
		}
	};


	class CameraV2
	{
	public:
		CameraV2(void) = delete;
		ENGINE_API CameraV2(
			Frustum const& frustum,
			math::Vector3<float> const& position = math::Vector3<float>(0.0f),
			float speed = 2.5f,
			float angularSpeed = 30.0f
		);

		ENGINE_API void Move(math::Vector3<float> dir);
		ENGINE_API void Rotate(math::Vector3<float> const& deltaAxis);
		ENGINE_API math::Matrix4<float> ViewProjection(void);

		ENGINE_API math::Vector3<float> GetPosition(void) const noexcept;
	
	private:
		math::Matrix4<float> GetViewMatrix(void);
		math::Matrix4<float> GetProjectionMatrix(void);
		float RotateAxis(float angle, float delta, float maxDelta);

		Frustum m_frustum;
		math::Matrix4<float> m_rotationMatrix;
		math::Vector3<float> m_position;
		math::Vector3<float> m_forward;
		math::Vector3<float> m_up;
		math::Vector3<float> m_right;
		math::Vector3<float> m_rotationAxis;

		float m_speed;
		float m_angularSpeed;
		float m_maxDeltaPitch;
		float m_maxDeltaRoll;
		float m_maxDeltaYaw;
		float m_deltaTime;
	};
}
