#include "camera/CameraV2.h"
#include "utility/Timer.h"

#include <math/Arithmetic.hpp>
#include <math/Vector4.hpp>

engine::CameraV2::CameraV2(Frustum const& frustum, math::Vector3<float> const& position, float speed, float angularSpeed)
	: m_frustum(frustum), m_position(position), m_speed(speed), m_angularSpeed(angularSpeed)
{
	m_forward = math::Vector3f::Front();
	m_right = math::Vector3f::Right();
	m_up = math::Vector3f::Up();
	m_projectionMatrix.Identity();
	m_rotation = math::Vector3f::Zero();

	GetProjectionMatrix();
}

void engine::CameraV2::Move(float x, float y, float z)
{
	math::Vector3<float> camSpaceDir = m_rotQuat.Rotate({x, y, z});
	m_position += camSpaceDir.Normalized() * m_speed * m_deltaTime;
}

void engine::CameraV2::Rotate(float deltaPitch, float deltaYaw, float deltaRoll)
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

math::Matrix4<float> engine::CameraV2::ViewProjection(void)
{
	m_deltaTime = DeltaTime();

	return m_projectionMatrix * GetViewMatrix();;
}

ENGINE_API math::Vector3<float> engine::CameraV2::GetPosition(void) const noexcept
{
	return m_position;
}

ENGINE_API math::Vector3<float> engine::CameraV2::GetRotation(void) const noexcept
{
	return m_rotation;
}

ENGINE_API float engine::CameraV2::GetSpeed(void) const noexcept
{
	return m_speed;
}

ENGINE_API float engine::CameraV2::GetRotationSpeed(void) const noexcept
{
	return m_angularSpeed;
}

ENGINE_API float engine::CameraV2::GetFOV(void) const noexcept
{
	return m_frustum.m_fovRad;
}

ENGINE_API float engine::CameraV2::GetNearPlane(void) const noexcept
{
	return m_frustum.m_near;
}

ENGINE_API float engine::CameraV2::GetFarPlane(void) const noexcept
{
	return m_frustum.m_far;
}

ENGINE_API math::Vector3<float>& engine::CameraV2::Position(void)
{
	return m_position;
}

ENGINE_API math::Vector3<float>& engine::CameraV2::Rotation(void)
{
	return m_rotation;
}

ENGINE_API float& engine::CameraV2::Speed(void)
{
	return m_speed;
}

ENGINE_API float& engine::CameraV2::RotationSpeed(void)
{
	return m_angularSpeed;
}

ENGINE_API void engine::CameraV2::SetFOV(float fov)
{
	m_frustum.m_fovRad = fov;

	GetProjectionMatrix();
}

ENGINE_API void engine::CameraV2::SetNearPlane(float nearPlane)
{
	m_frustum.m_near = nearPlane;

	GetProjectionMatrix();
}

ENGINE_API void engine::CameraV2::SetFarPlane(float farPlane)
{
	m_frustum.m_far = farPlane;

	GetProjectionMatrix();
}

math::Matrix4<float> engine::CameraV2::GetViewMatrix(void)
{
	math::Matrix4<float> matrix(1.0f);

	matrix[3][0] = -m_position.X();
	matrix[3][1] = -m_position.Y();
	matrix[3][2] = -m_position.Z();
	
	return m_rotQuat.RotationMatrix() * matrix;
}

void engine::CameraV2::GetProjectionMatrix(void)
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

float engine::CameraV2::RotateAxis(float angle, float delta)
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
