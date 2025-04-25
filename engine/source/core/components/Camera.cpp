#include "core/components/Camera.h"
#include "core/SceneGraph.h"
#include "core/systems/ScriptSystem.h"


#include "serialization/TextSerializer.h"

engine::Camera::Camera(EntityHandle owner, SceneGraph* scene)
{
    m_owner = owner;
    m_currentScene = scene;
    GetProjectionMatrix();
    
    

}

void engine::Camera::Move(const math::Vector3f& translation, f32 speed, f32 deltaTime)
{
    // Get or add transform


    math::Vector3f camSpaceDir = m_rotQuat.Rotate(translation);
    m_transform->SetPosition() += camSpaceDir.Normalized() * speed * deltaTime;
}

void engine::Camera::Rotate(f32 deltaPitch, f32 deltaYaw, f32 deltaRoll, f32 rotationSpeed)
{
    m_rotation[0] = RotateAxis(m_rotation[0], -deltaPitch, rotationSpeed);	// Pitch
    m_rotation[1] = RotateAxis(m_rotation[1], -deltaYaw, rotationSpeed);	// Yaw
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
    m_transform = m_currentScene->GetComponent<Transform>(m_owner);

    if (!m_transform)
        m_transform = m_currentScene->CreateComponent<Transform>(m_owner);

    return m_projectionMatrix * GetViewMatrix();
}

void engine::Camera::Register(void)
{
    engine::ScriptSystem::RegisterNewComponent("_NewCameraComponent", m_owner);
}

void engine::Camera::Unregister(void)
{
    ScriptSystem::UnregisterComponent("_RemoveCameraComponent", m_owner);
}

math::Vector3f engine::Camera::GetPosition(void) const noexcept
{
    return m_transform->SetPosition();
}

math::Vector3f engine::Camera::GetRotation(void) const noexcept
{
    return m_rotation;
}

math::Quatf engine::Camera::GetRotationQuat(void) const noexcept
{
    return m_rotQuat;
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
    return m_transform->SetPosition();
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

void engine::Camera::SerializeText(std::ostream& output, EntityHandle owner,
								   uint64 index) const
{
	output << "[Camera]\n    ";

	if constexpr (UpdateAfterParent<Camera>::m_value)
	{
		text::Serialize(output, "index", index);
		output << "\n    ";
	}
	text::Serialize(output, "owner", owner);
	output << "\n    ";
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
	//text::Serialize(output, "position", m_transform->SetPosition());
	//output << "\n    ";
	text::Serialize(output, "flags", m_flags);
	output << '\n';
}

const char* engine::Camera::DeserializeText(const char* text, const char* end)
{
	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeInteger(text, m_owner);

	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeReal(text, m_frustum.m_near);

	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeReal(text, m_frustum.m_far);

	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeReal(text, m_frustum.m_fovRad);

	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeReal(text, m_frustum.m_ratio);

	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeVector(text, m_rotation);

	//MOVE_TEXT_CURSOR(text, end);
	//text = text::DeserializeVector(text, m_transform->SetPosition());
    //text = text::GetNewLine(text, end);

	MOVE_TEXT_CURSOR(text, end);
	return text::DeserializeInteger(text, m_flags);
}

math::Matrix4f engine::Camera::GetViewMatrix(void)
{
    math::Matrix4f matrix(1.0f);
    math::Vector3f position = GetPosition();

    matrix[3][0] = -position.X();
    matrix[3][1] = -position.Y();
    matrix[3][2] = -position.Z();

    return m_rotQuat.Inverse().RotationMatrix() * matrix;
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
