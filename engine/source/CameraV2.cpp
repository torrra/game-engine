#include "camera/CameraV2.h"
#include "utility/Timer.h"

#include <math/Arithmetic.hpp>
#include <math/Vector4.hpp>
#include <math/Quaternion.hpp>

engine::CameraV2::CameraV2(Frustum const& frustum, math::Vector3<float> const& position, float speed, float angularSpeed)
	: m_frustum(frustum), m_position(position), m_speed(speed), m_angularSpeed(angularSpeed)
{
	m_forward = math::Vector3f::Front();
	m_right = math::Vector3f::Right();
	m_up = math::Vector3f::Up();
	m_rotationAxis = math::Vector3f::Zero();
	m_rotationMatrix.Identity();

	m_maxDeltaPitch = 5.0f;
	m_maxDeltaYaw = 5.0f;
	m_maxDeltaRoll = 5.0f;
}

void engine::CameraV2::Move(math::Vector3<float> dir)
{
	math::Matrix4f camSpace = GetViewMatrix();
	math::Vector4<float> camSpaceDir = camSpace * math::Vector4<float>(dir[0], dir[1], dir[2], 1.0f);
	math::Vector3<float> camSpaceDirVec3(camSpaceDir[0], camSpaceDir[1], camSpaceDir[2]);

	m_position += camSpaceDirVec3 * m_speed * m_deltaTime;
}

void engine::CameraV2::Rotate(math::Vector3<float> const& deltaAxis)
{
	static float pitch, yaw, roll = 0.0f;

	pitch = RotateAxis(pitch, deltaAxis[0], m_maxDeltaPitch);
	yaw = RotateAxis(yaw, deltaAxis[1], m_maxDeltaYaw);
	roll = RotateAxis(roll, deltaAxis[2], m_maxDeltaRoll);
	
	math::Quaternion<float> pitchQuat(math::Vector3f(1.0F, 0.0F, 0.0F), math::Radian(pitch * DEG2RAD));
	math::Quaternion<float>   yawQuat(math::Vector3f(0.0F, 1.0F, 0.0F), math::Radian(yaw * DEG2RAD));
	math::Quaternion<float>  rollQuat(math::Vector3f(0.0F, 0.0F, 0.1F), math::Radian(roll * DEG2RAD));
	math::Quaternion<float> combinedRotation = yawQuat * pitchQuat * rollQuat;

	m_rotationMatrix = combinedRotation.RotationMatrix();
}

math::Matrix4<float> engine::CameraV2::ViewProjection(void)
{
	m_deltaTime = DeltaTime();

	math::Matrix4<float> view = GetViewMatrix();
	math::Matrix4<float> projection = GetProjectionMatrix();
	
	return projection * view;
}

ENGINE_API math::Vector3<float> engine::CameraV2::GetPosition(void) const noexcept
{
	return m_position;
}

math::Matrix4<float> engine::CameraV2::GetViewMatrix(void)
{
	math::Vector3<float> forward = (m_forward).Normalized();
	m_right = (forward.Cross(m_up)).Normalized();
	math::Vector3<float> up = m_right.Cross(forward);

	const float viewValues[16] =
	{
		m_right[0], up[0], -forward[0], 0.0f,
		m_right[1], up[1], -forward[1], 0.0f,
		m_right[2], up[2], -forward[2], 0.0f,
		-(m_right.Dot(m_position)), -(up.Dot(m_position)), forward.Dot(m_position), 1.0f
	};

	return m_rotationMatrix * math::Matrix4<float>(viewValues);
}

math::Matrix4<float> engine::CameraV2::GetProjectionMatrix(void)
{
	static const float tanAngle = tanf(m_frustum.m_fovRad * 0.5f);
	const float farMinusNear = m_frustum.m_far - m_frustum.m_near;

	const float perspectiveValues[16] =
	{
		1.0f / (m_frustum.m_ratio * tanAngle), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / tanAngle, 0.0f, 0.0f,
		0.0f, 0.0f, -(m_frustum.m_far + m_frustum.m_near) / (farMinusNear),			-1.0f,
		0.0f, 0.0f, -((2.0f * m_frustum.m_far * m_frustum.m_near) / (farMinusNear)), 0.0f
	};

	return math::Matrix4<float>(perspectiveValues);
}

float engine::CameraV2::RotateAxis(float angle, float delta, float maxDelta)
{
	// Limits
	if (delta < -maxDelta)
		delta = -maxDelta;
	else if (delta > maxDelta)
		delta = maxDelta;

	angle += -delta * m_angularSpeed * m_deltaTime;

	// Wrap angle
	return math::Wrap<float>(angle, 0.0f, 360.0f);
}
