#include "engine/core/components/Camera.h"
#include "engine/core/components/Transform.h"
#include "engine/utility/Timer.h"

#include "engine/core/Entity.h"
#include "engine/core/SceneGraph.h"

#include <math/Arithmetic.hpp>
#include <math/Vector4.hpp>

engine::Camera::Camera(EntityHandle)
{
}

math::Matrix4f engine::Camera::ViewProjection(void)
{
	return m_projectionMatrix * GetViewMatrix();
}


f32 engine::Camera::GetFOV(void) const noexcept
{
	return m_frustum.m_fovRad;
}

f32 engine::Camera::GetNearPlane(void) const noexcept
{
	return m_frustum.m_near;
}

f32 engine::Camera::GetFarPlane(void) const noexcept
{
	return m_frustum.m_far;
}


void engine::Camera::SetFOV(f32 fov)
{
	m_frustum.m_fovRad = fov;

	CalcProjectionMatrix();
}

void engine::Camera::SetNearPlane(f32 nearPlane)
{
	m_frustum.m_near = nearPlane;

	CalcProjectionMatrix();
}

void engine::Camera::SetFarPlane(f32 farPlane)
{
	m_frustum.m_far = farPlane;

	CalcProjectionMatrix();
}

math::Matrix4f engine::Camera::GetViewMatrix(void)
{
	if(Transform* transform = m_currentScene->GetComponent<Transform>(m_owner))
	{
		math::Matrix4f matrix(1.0f);

		matrix[3][0] = -transform->GetPosition().X();
		matrix[3][1] = -transform->GetPosition().Y();
		matrix[3][2] = -transform->GetPosition().Z();

		return transform->GetRotation().RotationMatrix() * matrix;
	}
	
	return math::Matrix4f(1.f);
}

void engine::Camera::CalcProjectionMatrix(void)
{
	const f32 tanAngle = tanf(m_frustum.m_fovRad * 0.5f);
	const f32 farMinusNearDenom = 1.0f / (m_frustum.m_far - m_frustum.m_near);

	m_projectionMatrix[0][0] = 1.0f / (m_frustum.m_ratio * tanAngle);
	m_projectionMatrix[1][0] = 0.0f;
	m_projectionMatrix[2][0] = 0.0f;
	m_projectionMatrix[3][0] = 0.0f;

	m_projectionMatrix[0][1] = 0.0f;
	m_projectionMatrix[1][1] = 1.0f / tanAngle;
	m_projectionMatrix[2][1] = 0.0f;
	m_projectionMatrix[3][1] = 0.0f;

	m_projectionMatrix[0][2] = 0.0f;
	m_projectionMatrix[1][2] = 0.0f;
	m_projectionMatrix[2][2] = -(m_frustum.m_far + m_frustum.m_near) * farMinusNearDenom;
	m_projectionMatrix[3][2] = -((2.0f * m_frustum.m_far * m_frustum.m_near) * farMinusNearDenom);

	m_projectionMatrix[0][3] = 0.0f;
	m_projectionMatrix[1][3] = 0.0f;
	m_projectionMatrix[2][3] = -1.0f;
	m_projectionMatrix[3][3] = 0.0f;
}
