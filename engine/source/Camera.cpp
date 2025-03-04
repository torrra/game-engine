#include "camera/Camera.h"
#include "input/Input.h"

#include "math/Angle.hpp"
#include "math/Arithmetic.hpp"
#include "math/Matrix4.hpp"

#include <iostream>

engine::Camera::Camera(math::Vector3<float> position, float speed)
	: m_position(position), m_speed(speed), m_angularSpeed(5.0f)
{
	m_up = math::Vector3<float>::Up();
	m_forward = -math::Vector3<float>::Front();
	m_right = (m_forward.Cross(m_up)).Normalized();
}

math::Matrix4<float> engine::Camera::GetPerspectiveMatrix(float _near, float _far, float fovDeg, float aspect) const noexcept
{
	float fovRad = fovDeg * DEG2RAD;

	const float tanAngle = tanf(fovRad * 0.5f);
	const float farMinusNear = _far - _near;

	const float perspectiveValues[16] =
	{
		1.0f / (aspect * tanAngle), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / tanAngle, 0.0f, 0.0f,
		0.0f, 0.0f, -(_far + _near) / (farMinusNear), -1.0f,
		0.0f, 0.0f, -((2.0f * _far * _near) / (farMinusNear)), 0.0f
	};

	return math::Matrix4<float>(perspectiveValues);
}

math::Matrix4<float> engine::Camera::GetViewMatrix(void)
{
	// Camera forward direction
	math::Vector3<float> center = m_position + m_forward;
	math::Vector3<float> forward = (center - m_position).Normalized();
	m_right = (forward.Cross(m_up)).Normalized();
	math::Vector3<float> up = m_right.Cross(forward);

	const float viewValues[16] =
	{
		m_right[0], up[0], -forward[0], 0.0f,
		m_right[1], up[1], -forward[1], 0.0f,
		m_right[2], up[2], -forward[2], 0.0f,
		-(m_right.Dot(m_position)), -(up.Dot(m_position)), forward.Dot(m_position), 1.0f
	};

	return math::Matrix4<float>(viewValues);
}

math::Vector3<float> engine::Camera::GetPosition(void) const noexcept
{
	return m_position;
}

math::Vector3<float>& engine::Camera::GetPosition(void)
{
	return m_position;
}

void engine::Camera::CameraInput(float deltaTime)
{
	if (Input::IsInputHeld(KEY_W))
		m_position += m_forward * m_speed * deltaTime;
	else if (Input::IsInputHeld(KEY_S))
		m_position += -m_forward * m_speed * deltaTime;

	if (Input::IsInputHeld(KEY_A))
		m_position += -m_right * m_speed * deltaTime;
	else if (Input::IsInputHeld(KEY_D))
		m_position += m_right * m_speed * deltaTime;

	if (Input::IsInputHeld(KEY_Q))
		m_position -= math::Vector3<float>::Up() * m_speed * deltaTime;
	else if (Input::IsInputHeld(KEY_E))
		m_position += math::Vector3<float>::Up() * m_speed * deltaTime;


	//std::printf("pos: %f %f %f\n", m_position[0], m_position[1], m_position[2]);
}

void engine::Camera::MouseMotion(math::Vector2<float> const& cursorPos, float deltaTime)
{
	static math::Vector2<float> lastCursorPos(cursorPos);
	static float yaw, pitch = 0.0f;

	// Calculate delta mouse position
	math::Vector2<float> deltaPos = (cursorPos - lastCursorPos) * deltaTime;
	deltaPos *= 500.0f;
	// Prevent large numbers such as garbage data for yaw & pitch

	if (math::Absolute(deltaPos[0]) > 3.0f)
		deltaPos[0] = (deltaPos[0] < 0.0f) ? -3.0f : 3.0f;
	if (math::Absolute(deltaPos[1]) > 3.0f)
		deltaPos[1] = (deltaPos[1] < 0.0f) ? -3.0f : 3.0f;

	// Assign last cursor value
	lastCursorPos = cursorPos;

	// Update yaw & pitch
	yaw += deltaPos[0] * m_angularSpeed;
	pitch -= deltaPos[1] * m_angularSpeed;

	// Wrap yaw & pitch from 0 - 360 degrees
	yaw = fmodf(yaw, 360.0f);
	pitch = fmodf(pitch, 360.0f);

	// Convert degree to radian
	const float yawRad = yaw * DEG2RAD;
	const float pitchRad = pitch * DEG2RAD;

	// Rotate camera via pitch & yaw values
	m_forward = math::Vector3f(
		cosf(yawRad) * cosf(pitchRad),
		sinf(pitchRad),
		sinf(yawRad) * cosf(pitchRad)
	).Normalized();

	m_right = (m_forward.Cross(math::Vector3<float>::Up())).Normalized();
	m_up = (m_right.Cross(m_forward)).Normalized();
	std::printf("yaw: %f, pitch: %f\n", yaw, pitch);
}