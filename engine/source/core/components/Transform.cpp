#include "core/components/Transform.h"
#include "core/SceneGraph.h"

math::Matrix4f engine::Transform::ToMatrixWithoutScale(Transform& inTransform)
{
	math::Matrix4f transformMatrix = math::TransformMatrix(inTransform.m_rotation, inTransform.m_position);

	return transformMatrix;
}

math::Matrix4f engine::Transform::ToMatrixWithScale(Transform& inTransform)
{
	math::Matrix4f positionMatrix = math::Matrix4f::PositionMatrix(inTransform.m_position);

	math::Matrix4f rotationMatrix = inTransform.m_rotation.RotationMatrix();

	math::Matrix4f scaleMatrix = math::Matrix4f::ScaleMatrix(inTransform.m_scale);

	math::Matrix4f transformMatrix = (scaleMatrix * rotationMatrix) * positionMatrix;

	return transformMatrix;
}

engine::Transform engine::Transform::Interpolate(Transform& inStartTransform,
												 Transform& inEndTransform, f32 inTime)
{
	Transform result = Transform();

	result.m_position = math::Vector3f::Lerp(inStartTransform.m_position,
										   inEndTransform.m_position, inTime);

	result.m_rotation = math::Quatf::Slerp(inStartTransform.m_rotation,
										 inEndTransform.m_rotation, inTime);

	result.m_scale = math::Vector3f::Lerp(inStartTransform.m_scale,
										inEndTransform.m_scale, inTime);

	return result;
}

void engine::Transform::CopyPosition(const Transform& inTransform)
{
	m_position = inTransform.m_position;
}

void engine::Transform::CopyRotation(const Transform& inTransform)
{
	m_rotation = inTransform.m_rotation;
}

void engine::Transform::CopyScale(const Transform& inTransform)
{
	m_scale = inTransform.m_scale;
}

void engine::Transform::Update(void)
{
	m_parentCache.m_cached = false;
}

void engine::Transform::Move(math::Vector3f translation)
{
	m_position += m_rotation.Rotate(translation);
}

void engine::Transform::Rotate(f32 angleX, f32 angleY, f32 angleZ)
{
	Rotate(math::Quatf(math::Radian(-angleX * DEG2RAD),
					   math::Radian(-angleY * DEG2RAD), 
					   math::Radian(-angleZ * DEG2RAD)));

}

void engine::Transform::Rotate(const math::Quatf& rotation)
{
	m_rotation *= rotation.Normalized();
	UpdateAxes();
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

math::Vector3f engine::Transform::GetAbsolutePosition(void)
{
	if (!m_parentCache.m_cached)
		CacheParentTransform();

	return m_parentCache.m_position + m_position;
}

math::Vector3f engine::Transform::GetAbsoluteScale(void)
{
	if (!m_parentCache.m_cached)
		CacheParentTransform();

	return m_parentCache.m_scale * m_scale;
}

math::Quatf engine::Transform::GetAbsoluteRotation(void)
{
	if (!m_parentCache.m_cached)
		CacheParentTransform();

	return m_parentCache.m_rotation * m_rotation;
}

void engine::Transform::SetPosition(const math::Vector3f& inPosition)
{
	m_position = inPosition;
}

void engine::Transform::SetRotation(const math::Quatf& inRotation)
{
	m_rotation = inRotation;
	UpdateAxes();
}

void engine::Transform::SetRotation(f32 angleX, f32 angleY, f32 angleZ)
{
	SetRotation(math::Quatf(math::Radian(angleX * DEG2RAD),
							math::Radian(angleY * DEG2RAD),
							math::Radian(angleZ * DEG2RAD)));

}

void engine::Transform::SetScale(const math::Vector3f& inScale)
{
	m_scale = inScale;
}

void engine::Transform::SetTransform(const math::Vector3f& inPosition,
	const math::Quatf& inRotation, const math::Vector3f& inScale)
{
	m_position = inPosition;
	m_rotation = inRotation;
	m_scale = inScale;
}

std::ostream& engine::Transform::operator<<(std::ostream& os)
{
	return os << m_position.X() << " " << m_position.Y() << " " << m_position.Z() << " "
			  << m_rotation.W() << " " << m_rotation.X() << " " << m_rotation.Y() << " " 
			  << m_rotation.Z() << " "
			  << m_scale.X() << " " << m_scale.Y() << " " << m_scale.Z();
}

void engine::Transform::UpdateAxes(void)
{
	m_forward = m_rotation.Rotate(math::Vector3f::Front());

	if (m_forward.Magnitude() != 0.f)
		m_forward.Normalize();

	m_right = math::Cross(m_forward, math::Vector3f::Up());

	if (m_right.Magnitude() != 0.f)
		m_right.Normalize();

	m_up = math::Cross(m_right, m_forward);

	if (m_up.Magnitude() != 0.f)
		m_up.Normalize();

}

void engine::Transform::CacheParentTransform(void)
{
	std::vector<EntityHandle> parents = m_currentScene->GetAllParents(m_owner);

	m_parentCache.m_position = math::Vector3f::Zero();
	m_parentCache.m_scale = math::Vector3f::Zero();
	m_parentCache.m_rotation = math::Quatf(1.f, 0.f, 0.f, 0.f);

	// Iterate backwards to get parent closest to root first
	for (auto parentIt = parents.rbegin(); parentIt != parents.rend(); ++parentIt)
	{
		if (Transform* transform = m_currentScene->GetComponent<Transform>(*parentIt))
		{
			m_parentCache.m_position += transform->GetAbsolutePosition();
			m_parentCache.m_scale *= transform->GetAbsoluteScale();
			m_parentCache.m_rotation *= transform->GetAbsoluteRotation();
		}
	}

	m_parentCache.m_cached = true;
}