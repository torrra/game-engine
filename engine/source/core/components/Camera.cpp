#include "core/components/Camera.h"
#include "utility/Timer.h"

#include <math/Arithmetic.hpp>
#include <math/Vector4.hpp>

engine::Camera::Camera(Frustum const& frustum, math::Vector3<float> const& position, float speed, float angularSpeed)
	: m_frustum(frustum), m_position(position), m_speed(speed), m_angularSpeed(angularSpeed)
{
	m_forward = math::Vector3f::Front();
	m_right = math::Vector3f::Right();
	m_up = math::Vector3f::Up();
	m_projectionMatrix.Identity();
	m_rotation = math::Vector3f::Zero();

	GetProjectionMatrix();
}

void engine::Camera::Move(float x, float y, float z)
{
	math::Vector3<float> camSpaceDir = m_rotQuat.Rotate({x, y, z});
	m_position += camSpaceDir.Normalized() * m_speed * m_deltaTime;
}

void engine::Camera::Rotate(float deltaPitch, float deltaYaw, float deltaRoll)
{
	m_rotation[0] = RotateAxis(m_rotation[0], -deltaPitch);	// Pitch
	m_rotation[1] = RotateAxis(m_rotation[1], -deltaYaw);		// Yaw
	m_rotation[2] = RotateAxis(m_rotation[2], -deltaRoll);		// Roll

	// Clamp pitch // TODO: fix pitch
	if (m_rotation[0] > 90.0f)
		m_rotation[0] = 90.0f;
	else if (m_rotation[0] < -90.0f)
		m_rotation[0] = -90.0f;

	m_rotQuat = math::Quaternion<float>(
		math::Radian(m_rotation[0] * DEG2RAD),
		math::Radian(m_rotation[1] * DEG2RAD),
		math::Radian(m_rotation[2] * DEG2RAD)
	);
}

math::Matrix4<float> engine::Camera::ViewProjection(void)
{
	m_deltaTime = g_engineTime.GetDeltaTime();

	return m_projectionMatrix * GetViewMatrix();
}

ENGINE_API math::Vector3<float> engine::Camera::GetPosition(void) const noexcept
{
	return m_position;
}

ENGINE_API math::Vector3<float> engine::Camera::GetRotation(void) const noexcept
{
	return m_rotation;
}

ENGINE_API float engine::Camera::GetSpeed(void) const noexcept
{
	return m_speed;
}

ENGINE_API float engine::Camera::GetRotationSpeed(void) const noexcept
{
	return m_angularSpeed;
}

ENGINE_API float engine::Camera::GetFOV(void) const noexcept
{
	return m_frustum.m_fovRad;
}

ENGINE_API float engine::Camera::GetNearPlane(void) const noexcept
{
	return m_frustum.m_near;
}

ENGINE_API float engine::Camera::GetFarPlane(void) const noexcept
{
	return m_frustum.m_far;
}

ENGINE_API math::Vector3<float>& engine::Camera::Position(void)
{
	return m_position;
}

ENGINE_API math::Vector3<float>& engine::Camera::Rotation(void)
{
	return m_rotation;
}

ENGINE_API float& engine::Camera::Speed(void)
{
	return m_speed;
}

ENGINE_API float& engine::Camera::RotationSpeed(void)
{
	return m_angularSpeed;
}

ENGINE_API void engine::Camera::SetFOV(float fov)
{
	m_frustum.m_fovRad = fov;

	GetProjectionMatrix();
}

ENGINE_API void engine::Camera::SetNearPlane(float nearPlane)
{
	m_frustum.m_near = nearPlane;

	GetProjectionMatrix();
}

ENGINE_API void engine::Camera::SetFarPlane(float farPlane)
{
	m_frustum.m_far = farPlane;

	GetProjectionMatrix();
}

math::Matrix4<float> engine::Camera::GetViewMatrix(void)
{
	math::Matrix4<float> matrix(1.0f);

	matrix[3][0] = -m_position.X();
	matrix[3][1] = -m_position.Y();
	matrix[3][2] = -m_position.Z();
	
	return m_rotQuat.RotationMatrix() * matrix;
}

void engine::Camera::GetProjectionMatrix(void)
{
	const float tanAngle = tanf(m_frustum.m_fovRad * 0.5f);
	const float farMinusNearDenom = 1.0f / (m_frustum.m_far - m_frustum.m_near);

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

float engine::Camera::RotateAxis(float angle, float delta)
{
	float maxDelta = 5.0f;

	// Limits
	if (delta < -maxDelta)
		delta = -maxDelta;
	else if (delta > maxDelta)
		delta = maxDelta;

	angle += delta * m_angularSpeed * m_deltaTime;

	// Wrap angle
	return angle;
}
