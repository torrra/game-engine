#pragma once

#include <math/Vector3.hpp>
#include <math/Matrix4.hpp>
#include <math/Quaternion.hpp>
#include "engine/EngineExport.h"

namespace engine
{
	struct Frustum
	{
		float m_near;
		float m_far;
		float m_fovRad;
		float m_ratio;
		
		ENGINE_API Frustum(float near = 0.005f, float far = 250.0f, float fovDeg = 60.0f, float ratio = 1.77777777778f)
			: m_near(near), m_far(far), m_fovRad(fovDeg * DEG2RAD), m_ratio(ratio)
		{
		}
	};


	class Camera
	{
	public:
		Camera(void) = delete;
		ENGINE_API Camera(
			Frustum const& frustum,
			math::Vector3<float> const& position = math::Vector3<float>(0.0f),
			float speed = 2.5f,
			float angularSpeed = 30.0f
		);

		ENGINE_API ~Camera(void) = default;

		ENGINE_API void Move(float x, float y, float z);
		ENGINE_API void Rotate(float deltaPitch, float deltaYaw, float deltaRoll);
		ENGINE_API math::Matrix4<float> ViewProjection(void);

		ENGINE_API math::Vector3<float> GetPosition(void) const noexcept;
		ENGINE_API math::Vector3<float> GetRotation(void) const noexcept;
		ENGINE_API float GetSpeed(void) const noexcept;
		ENGINE_API float GetRotationSpeed(void) const noexcept;
		ENGINE_API float GetFOV(void) const noexcept;
		ENGINE_API float GetNearPlane(void) const noexcept;
		ENGINE_API float GetFarPlane(void) const noexcept;

		ENGINE_API math::Vector3<float>& Position(void);
		ENGINE_API math::Vector3<float>& Rotation(void);
		ENGINE_API float& Speed(void);
		ENGINE_API float& RotationSpeed(void);
		ENGINE_API void SetFOV(float fov);
		ENGINE_API void SetNearPlane(float nearPlane);
		ENGINE_API void SetFarPlane(float farPlane);
	
	private:
		math::Matrix4<float> GetViewMatrix(void);
		void GetProjectionMatrix(void);
		float RotateAxis(float angle, float delta);

		Frustum m_frustum;
		math::Quaternion<float> m_rotQuat;
		math::Matrix4<float> m_projectionMatrix;
		math::Vector3<float> m_position;
		math::Vector3<float> m_rotation;
		math::Vector3<float> m_forward;
		math::Vector3<float> m_up;
		math::Vector3<float> m_right;

		float m_speed;
		float m_angularSpeed;
		float m_deltaTime;
	};
}
