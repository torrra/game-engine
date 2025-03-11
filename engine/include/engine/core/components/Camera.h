#pragma once

#include <math/Vector3.hpp>
#include <math/Matrix4.hpp>
#include <math/Quaternion.hpp>

#include "engine/core/Component.h"

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

	class Camera : public Component
	{
	public:
		
		using Component::Component;

		ENGINE_API ~Camera(void) = default;

		ENGINE_API void Register(void) {};

		ENGINE_API math::Matrix4f ViewProjection(void);

		ENGINE_API f32 GetFOV(void) const noexcept;
		ENGINE_API f32 GetNearPlane(void) const noexcept;
		ENGINE_API f32 GetFarPlane(void) const noexcept;

		ENGINE_API void SetFOV(f32 fov);
		ENGINE_API void SetNearPlane(f32 nearPlane);
		ENGINE_API void SetFarPlane(f32 farPlane);
	
	private:

		math::Matrix4f GetViewMatrix(void);
		void CalcProjectionMatrix(void);

		Frustum m_frustum;
		math::Matrix4f m_projectionMatrix;
	};
}
