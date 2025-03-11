#include "core/components/Transform.h"
#include "core/SceneGraph.h"

lm::Matrix4f engine::Transform::ToMatrixWithoutScale(Transform& inTransform)
{
	lm::Matrix4f transformMatrix = lm::TransformMatrix(inTransform.m_rotation, inTransform.m_position);

	return transformMatrix;
}

lm::Matrix4f engine::Transform::ToMatrixWithScale(Transform& inTransform)
{
	lm::Matrix4f positionMatrix = lm::Matrix4f::PositionMatrix(inTransform.m_position);

	lm::Matrix4f rotationMatrix = inTransform.m_rotation.RotationMatrix();

	lm::Matrix4f scaleMatrix = lm::Matrix4f::ScaleMatrix(inTransform.m_scale);

	lm::Matrix4f transformMatrix = (scaleMatrix * rotationMatrix) * positionMatrix;

	return transformMatrix;
}

engine::Transform engine::Transform::Interpolate(Transform& inStartTransform,
												 Transform& inEndTransform, f32 inTime)
{
	Transform result = Transform();

	result.m_position = lm::Vector3f::Lerp(inStartTransform.m_position,
										   inEndTransform.m_position, inTime);

	result.m_rotation = lm::Quatf::Slerp(inStartTransform.m_rotation,
										 inEndTransform.m_rotation, inTime);

	result.m_scale = lm::Vector3f::Lerp(inStartTransform.m_scale,
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

void engine::Transform::Move(math::Vector3f translation)
{
	m_position += m_forward * translation;
}

void engine::Transform::Rotate(f32 angleX, f32 angleY, f32 angleZ)
{
	Rotate(math::Quatf(math::Radian(angleX),
					   math::Radian(angleY), 
					   math::Radian(angleZ)));

}

void engine::Transform::Rotate(const math::Quatf& rotation)
{
	m_rotation *= rotation;
	UpdateAxes();
}

lm::Vector3f engine::Transform::GetPosition(void) const
{
	return m_position;
}

lm::Quatf engine::Transform::GetRotation(void) const
{
	return m_rotation;
}

lm::Vector3f engine::Transform::GetScale(void) const
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

void engine::Transform::SetPosition(const lm::Vector3f& inPosition)
{
	m_position = inPosition;
}

void engine::Transform::SetRotation(const lm::Quatf& inRotation)
{
	m_rotation = inRotation;
}

void engine::Transform::SetScale(const lm::Vector3f& inScale)
{
	m_scale = inScale;
}

void engine::Transform::SetTransform(const lm::Vector3f& inPosition,
	const lm::Quatf& inRotation, const lm::Vector3f& inScale)
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
	m_forward.Normalize();

	m_right = math::Cross(m_forward, math::Vector3f::Up());
	m_right.Normalize();

	m_up = math::Cross(m_right, m_forward);
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