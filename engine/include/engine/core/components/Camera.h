#pragma once

#include <math/Vector3.hpp>
#include <math/Matrix4.hpp>
#include <math/Quaternion.hpp>

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

namespace engine
{
	struct Frustum
	{
		f32 m_near;
		f32 m_far;
		f32 m_fovRad;
		f32 m_ratio;
		
		ENGINE_API Frustum(f32 near = 0.005f, f32 far = 250.0f, f32 fovDeg = 60.0f, f32 ratio = 1.77777777778f)
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
			math::Vector3f const& position = math::Vector3f(0.0f),
			f32 speed = 2.5f,
			f32 angularSpeed = 30.0f
		);

		ENGINE_API ~Camera(void) = default;

		ENGINE_API void Move(f32 x, f32 y, f32 z);
		ENGINE_API void Rotate(f32 deltaPitch, f32 deltaYaw, f32 deltaRoll);
		ENGINE_API math::Matrix4f ViewProjection(void);

		ENGINE_API math::Vector3f GetPosition(void) const noexcept;
		ENGINE_API math::Vector3f GetRotation(void) const noexcept;
		ENGINE_API f32 GetSpeed(void) const noexcept;
		ENGINE_API f32 GetRotationSpeed(void) const noexcept;
		ENGINE_API f32 GetFOV(void) const noexcept;
		ENGINE_API f32 GetNearPlane(void) const noexcept;
		ENGINE_API f32 GetFarPlane(void) const noexcept;

		ENGINE_API math::Vector3f& Position(void);
		ENGINE_API math::Vector3f& Rotation(void);
		ENGINE_API f32& Speed(void);
		ENGINE_API f32& RotationSpeed(void);
		ENGINE_API void SetFOV(f32 fov);
		ENGINE_API void SetNearPlane(f32 nearPlane);
		ENGINE_API void SetFarPlane(f32 farPlane);
	
	private:
		math::Matrix4f GetViewMatrix(void);
		void GetProjectionMatrix(void);
		f32 RotateAxis(f32 angle, f32 delta);

		Frustum m_frustum;
		math::Quaternion<f32> m_rotQuat;
		math::Matrix4f m_projectionMatrix;
		math::Vector3f m_position;
		math::Vector3f m_rotation;
		math::Vector3f m_forward;
		math::Vector3f m_up;
		math::Vector3f m_right;

		f32 m_speed;
		f32 m_angularSpeed;
		f32 m_deltaTime;
	};
}
