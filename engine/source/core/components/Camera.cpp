#include "core/components/Camera.h"
#include "core/systems/ScriptSystem.h"
#include "utility/Timer.h"

#include <math/Arithmetic.hpp>
#include <math/Vector4.hpp>

#include <fstream>
#include "serialization/TextSerializer.h"

engine::Camera::Camera(EntityHandle owner, SceneGraph* scene)
{
	m_owner = owner;
	m_currentScene = scene;
	GetProjectionMatrix();
}

void engine::Camera::Move(const math::Vector3f& translation, f32 speed, f32 deltaTime)
{
	math::Vector3f camSpaceDir = m_rotQuat.Rotate(translation);
	m_position += camSpaceDir.Normalized() * speed * deltaTime;
}

void engine::Camera::Rotate(f32 deltaPitch, f32 deltaYaw, f32 deltaRoll, f32 rotationSpeed)
{
	m_rotation[0] = RotateAxis(m_rotation[0], -deltaPitch, rotationSpeed);	// Pitch
	m_rotation[1] = RotateAxis(m_rotation[1], -deltaYaw, rotationSpeed);		// Yaw
	m_rotation[2] = RotateAxis(m_rotation[2], -deltaRoll, rotationSpeed);	// Roll

	// Clamp pitch // TODO: fix pitch
	if (m_rotation[0] > 90.0f)
		m_rotation[0] = 90.0f;
	else if (m_rotation[0] < -90.0f)
		m_rotation[0] = -90.0f;

	m_rotQuat = math::Quaternion<f32>(
		math::Radian(m_rotation[0] * DEG2RAD),
		math::Radian(m_rotation[1] * DEG2RAD),
		math::Radian(m_rotation[2] * DEG2RAD)
	);
}

math::Matrix4f engine::Camera::ViewProjection(void)
{
	return m_projectionMatrix * GetViewMatrix();
}

void engine::Camera::Register(void)
{
	engine::ScriptSystem::RegisterNewComponent("_NewCameraComponent", m_owner);
}

math::Vector3f engine::Camera::GetPosition(void) const noexcept
{
	return m_position;
}

math::Vector3f engine::Camera::GetRotation(void) const noexcept
{
	return m_rotation;
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

math::Vector3f& engine::Camera::Position(void)
{
	return m_position;
}

math::Vector3f& engine::Camera::Rotation(void)
{
	return m_rotation;
}


void engine::Camera::SetFOV(f32 fov)
{
	m_frustum.m_fovRad = fov;

	GetProjectionMatrix();
}

void engine::Camera::SetNearPlane(f32 nearPlane)
{
	m_frustum.m_near = nearPlane;

	GetProjectionMatrix();
}

void engine::Camera::SetFarPlane(f32 farPlane)
{
	m_frustum.m_far = farPlane;

	GetProjectionMatrix();
}

void engine::Camera::SerializeText(std::ofstream& output)
{
	output << "[Camera]\n    ";
	text::Serialize(output, "near", m_frustum.m_near);
	output << "\n    ";
	text::Serialize(output, "far", m_frustum.m_far);
	output << "\n    ";
	text::Serialize(output, "fov", m_frustum.m_fovRad);
	output << "\n    ";
	text::Serialize(output, "aspectRatio", m_frustum.m_ratio);
	output << "\n    ";
	text::Serialize(output, "rotationEuler", m_rotation);
	output << "\n    ";
	text::Serialize(output, "position", m_position);
	output << "\n    ";
	text::Serialize(output, "flags", m_flags);
	output << '\n';
}

math::Matrix4f engine::Camera::GetViewMatrix(void)
{
	math::Matrix4f matrix(1.0f);

	matrix[3][0] = -m_position.X();
	matrix[3][1] = -m_position.Y();
	matrix[3][2] = -m_position.Z();

	return m_rotQuat.RotationMatrix() * matrix;
}

void engine::Camera::GetProjectionMatrix(void)
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

f32 engine::Camera::RotateAxis(f32 existingAngle, f32 deltaAngle, f32 rotationSpeed)
{
	f32 maxDelta = 5.0f;

	// Limits
	if (deltaAngle < -maxDelta)
		deltaAngle = -maxDelta;
	else if (deltaAngle > maxDelta)
		deltaAngle = maxDelta;

	existingAngle += deltaAngle * rotationSpeed;

	// Wrap angle
	return existingAngle;
}
