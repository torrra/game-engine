#include "core/components/Transform.h"
#include "core/systems/ScriptSystem.h"
#include "core/SceneGraph.h"

#include "serialization/TextSerializer.h"

math::Matrix4f engine::Transform::ToMatrixWithoutScale(const Transform& inTransform)
{
    math::Matrix4f transformMatrix = math::TransformMatrix(inTransform.m_rotation, inTransform.m_position);

    return transformMatrix;
}

math::Matrix4f engine::Transform::ToMatrixWithScale(Transform& inTransform)
{
    if (inTransform.m_dirty)
        inTransform.UpdateLocalMatrix();

    return inTransform.m_localMat;
}

engine::Transform engine::Transform::Interpolate(const Transform& inStartTransform,
                                                 const Transform& inEndTransform, f32 inTime)
{
    Transform result = Transform();

    result.m_position = math::Vector3f::Lerp(inStartTransform.m_position,
                                             inEndTransform.m_position, inTime);

    result.m_rotation = math::Quatf::Slerp(inStartTransform.m_rotation,
                                           inEndTransform.m_rotation, inTime);

    result.m_scale = math::Vector3f::Lerp(inStartTransform.m_scale,
                                          inEndTransform.m_scale, inTime);

    result.m_dirty = true;

    return result;
}


math::Matrix4f engine::Transform::ToWorldMatrix(Transform& inTransform)
{
    math::Matrix4f world{ 1.f };

    std::vector<EntityHandle> parents = 
    inTransform.m_currentScene->GetAllParents(inTransform.m_owner);

    // Iterate backwards to get parent closest to root first
    for (auto parentIt = parents.begin(); parentIt != parents.end(); ++parentIt)
    {
        if (Transform* transform = inTransform.m_currentScene->GetComponent<Transform>(*parentIt))
            world = ToMatrixWithScale(*transform) * world;
    }

    return world * ToMatrixWithScale(inTransform);
}

math::Vector3f engine::Transform::ToWorldPosition(const Transform& inTransform)
{
    math::Vector3f worldPosition = inTransform.m_position;

    std::vector<EntityHandle> parents =
        inTransform.m_currentScene->GetAllParents(inTransform.m_owner);

    for (auto parentIt = parents.begin(); parentIt != parents.end(); ++parentIt)
    {
        if (Transform* transform = inTransform.m_currentScene->GetComponent<Transform>(*parentIt))
        {
            worldPosition = worldPosition * transform->m_scale;
            worldPosition = transform->m_rotation * worldPosition;
            worldPosition += transform->m_position;
        }
    }

    return worldPosition;
}

math::Quatf engine::Transform::ToWorldRotation(const Transform& inTransform)
{
    math::Quatf worldRotation = inTransform.m_rotation;

    std::vector<EntityHandle> parents =
        inTransform.m_currentScene->GetAllParents(inTransform.m_owner);

    for (auto parentIt = parents.begin(); parentIt != parents.end(); ++parentIt)
    {
        if (Transform* transform = inTransform.m_currentScene->GetComponent<Transform>(*parentIt))
            worldRotation = transform->m_rotation * inTransform.m_rotation;
    }

    return worldRotation;
}

void engine::Transform::CopyPosition(const Transform& inTransform)
{
    m_position = inTransform.m_position;
    m_dirty = true;
}

void engine::Transform::CopyRotation(const Transform& inTransform)
{
    m_rotation = inTransform.m_rotation;
    m_dirty = true;
}

void engine::Transform::CopyScale(const Transform& inTransform)
{
    m_scale = inTransform.m_scale;
    m_dirty = true;
}

void engine::Transform::Update(const Transform& inTransform)
{
    m_position = inTransform.m_position;
    m_rotation = inTransform.m_rotation;
    m_scale = inTransform.m_scale;
    m_dirty = true;
}

void engine::Transform::Register(void)
{
    engine::ScriptSystem::RegisterNewComponent("_NewTransformComponent", m_owner);
}

void engine::Transform::Unregister(void)
{
    ScriptSystem::UnregisterComponent("_RemoveTransformComponent", m_owner);
}

math::Vector3f engine::Transform::GetPosition(void) const
{
    return m_position;
}

math::Quatf engine::Transform::GetRotation(void) const
{
    return m_rotation;
}

math::Vector3f engine::Transform::GetScale(void) const
{
    return m_scale;
}

engine::Transform engine::Transform::GetTransform(void) const
{
    return *this;
}

engine::Transform& engine::Transform::SetPosition(const math::Vector3f& inPosition)
{
    m_position = inPosition;
    m_dirty = true;

    return *this;
}

math::Vector3f& engine::Transform::SetPosition(void)
{
    m_dirty = true;

    return m_position;
}

math::Vector3f engine::Transform::GetEulerRotation(void) const noexcept
{
    return m_rotation.EulerAngles() * RAD2DEG;
}

math::Vector3f& engine::Transform::SetScale(void)
{
    m_dirty = true;

    return m_scale;
}

engine::Transform& engine::Transform::SetRotation(const math::Quatf& inRotation)
{
    m_rotation = inRotation.Normalized();
    m_dirty = true;

    return *this;
}

engine::Transform& engine::Transform::SetScale(const math::Vector3f& inScale)
{
    m_scale = inScale;
    m_dirty = true;

    return *this;
}

engine::Transform& engine::Transform::SetTransform(const Transform& inTransform)
{
    m_position = inTransform.m_position;
    m_rotation = inTransform.m_rotation;
    m_scale = inTransform.m_scale;
    m_dirty = true;

    return *this;
}

engine::Transform& engine::Transform::SetTransform(const math::Vector3f& inPosition,
                                                   const math::Quatf& inRotation,
                                                   const math::Vector3f& inScale)
{
    m_position = inPosition;
    m_rotation = inRotation;
    m_scale = inScale;
    m_dirty = true;

    return *this;
}


void engine::Transform::AddTranslation(const math::Vector3f& translation)
{
    m_position += translation;
    m_dirty = true;
}


void engine::Transform::AddRotation(f32 angleX, f32 angleY, f32 angleZ)
{
    math::Quatf quatRotation
    {
        math::Radian(angleX * DEG2RAD), math::Radian(angleY * DEG2RAD), math::Radian(angleZ * DEG2RAD)
    };

    AddRotation(quatRotation);
}

void engine::Transform::AddRotation(const math::Quatf& rotation)
{
    m_rotation = rotation * m_rotation;
    m_dirty = true;
}

void engine::Transform::AddScale(const math::Vector3f& scale)
{
    m_scale *= scale;
    m_dirty = true;
}

void engine::Transform::SerializeText(std::ostream& output, EntityHandle owner,
									 uint64 index) const
{
	output << "[Transform]\n    ";

	if constexpr (UpdateAfterParent<Transform>::m_value)
	{
		text::Serialize(output, "index", index);
		output << "\n    ";
	}


	text::Serialize(output, "owner", owner);
	output << "\n    ";
	text::Serialize(output, "rotation", m_rotation);
	output << "\n    ";
	text::Serialize(output, "position", m_position);
	output << "\n    ";
	text::Serialize(output, "scale", m_scale);
	output << "\n    ";
	text::Serialize(output, "flags", m_flags);
	output << '\n';
}

const char* engine::Transform::DeserializeText(const char* text, const char* end)
{
	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeInteger(text, m_owner);

	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeQuaternion(text, m_rotation);

	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeVector(text, m_position);

	MOVE_TEXT_CURSOR(text, end);
	text = text::DeserializeVector(text, m_scale);

	MOVE_TEXT_CURSOR(text, end);
	return text::DeserializeInteger(text, m_flags);
	
}

std::ostream& engine::Transform::operator<<(std::ostream& os)
{
    return os << m_position.X() << " " << m_position.Y() << " " << m_position.Z() << " "
              << m_rotation.W() << " " << m_rotation.X() << " " << m_rotation.Y() << " "
              << m_rotation.Z() << " "
              << m_scale.X() << " " << m_scale.Y() << " " << m_scale.Z();
}

std::ostream& engine::operator<<(std::ostream& os, engine::Transform& transform)
{
	return os << "position: " << transform.m_position.X() << " " << transform.m_position.Y() << " " << transform.m_position.Z() << "\nrotation: "
		<< transform.m_rotation.W() << " " << transform.m_rotation.X() << " " << transform.m_rotation.Y() << " "
		<< transform.m_rotation.Z() << "\nscale: "
		<< transform.m_scale.X() << " " << transform.m_scale.Y() << " " << transform.m_scale.Z() << '\n';
}

void engine::Transform::UpdateLocalMatrix(void)
{
    m_localMat = math::TransformMatrix(m_rotation, m_position, m_scale);
    m_dirty = false;
}
