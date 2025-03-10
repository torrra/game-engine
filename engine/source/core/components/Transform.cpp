#include "core/components/Transform.h"

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
												 Transform& inEndTransform, float inTime)
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
