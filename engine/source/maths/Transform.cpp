#include "../../engine/include/maths/Transform.h"

Engine::Transform::Transform(const lm::Vector3f& inPosition)
{
	m_position	= inPosition;
	m_rotation	= lm::Quatf(1.f, 0.f, 0.f, 0.f);
	m_scale		= lm::Vector3f(1.f, 1.f, 1.f);
}

Engine::Transform::Transform(const lm::Quatf& inRotation)
{
	m_position	= lm::Vector3f(0.f, 0.f, 0.f);
	m_rotation	= inRotation;
	m_scale		= lm::Vector3f(1.f, 1.f, 1.f);
}

Engine::Transform::Transform(const lm::Vector3f& inPosition, const lm::Quatf& inRotation, 
							 const lm::Vector3f& inScale)
{
	m_position	= inPosition;
	m_rotation	= inRotation;
	m_scale		= inScale;
}

lm::Matrix4f Engine::Transform::ToMatrixWithoutScale(Transform& inTransform)
{
	lm::Matrix4f positionMatrix = lm::Matrix4f::Matrix(1.f);
	positionMatrix.PositionMatrix(inTransform.m_position);

	lm::Matrix4f rotationMatrix = inTransform.m_rotation.RotationMatrix();

	lm::Matrix4f transformMatrix = rotationMatrix * positionMatrix;

	return transformMatrix;
}

lm::Matrix4f Engine::Transform::ToMatrixWithScale(Transform& inTransform)
{
	lm::Matrix4f positionMatrix = lm::Matrix4f::Matrix(1.f);
	positionMatrix.PositionMatrix(inTransform.m_position);

	lm::Matrix4f rotationMatrix = inTransform.m_rotation.RotationMatrix();

	lm::Matrix4f scaleMatrix = lm::Matrix4f::Matrix(1.f);
	scaleMatrix.ScaleMatrix(inTransform.m_scale);

	lm::Matrix4f transformMatrix = (scaleMatrix * rotationMatrix) * positionMatrix;

	return transformMatrix;
}

Engine::Transform Engine::Transform::Interpolate(Transform& inStartTransform,
												 Transform& inEndTransform, float inTime)
{
	Transform result = Transform();

	result.m_position	= lm::Vector3f::Lerp(inStartTransform.m_position, 
											 inEndTransform.m_position, inTime);

	result.m_rotation	= lm::Quatf::Slerp(inStartTransform.m_rotation, 
										   inEndTransform.m_rotation, inTime);

	result.m_scale		= lm::Vector3f::Lerp(inStartTransform.m_scale, 
											 inEndTransform.m_scale, inTime);

	return result;
}

void Engine::Transform::CopyPosition(const Transform& inTransform)
{
	m_position = inTransform.m_position;
}

void Engine::Transform::CopyRotation(const Transform& inTransform)
{
	m_rotation = inTransform.m_rotation;
}

void Engine::Transform::CopyScale(const Transform& inTransform)
{
	m_scale = inTransform.m_scale;
}

lm::Vector3f Engine::Transform::GetPosition(void) const
{
	return m_position;
}

lm::Quatf Engine::Transform::GetRotation(void) const
{
	return m_rotation;
}

lm::Vector3f Engine::Transform::GetScale(void) const
{
	return m_scale;
}

Engine::Transform Engine::Transform::GetTransform(void) const
{
	return *this;
}

void Engine::Transform::SetPosition(const lm::Vector3f& inPosition)
{
	m_position = inPosition;
}

void Engine::Transform::SetRotation(const lm::Quatf& inRotation)
{
	m_rotation = inRotation;
}

void Engine::Transform::SetScale(const lm::Vector3f& inScale)
{
	m_scale = inScale;
}

void Engine::Transform::SetTransform(const lm::Vector3f& inPosition, 
									 const lm::Quatf& inRotation, const lm::Vector3f& inScale)
{
	m_position	= inPosition;
	m_rotation	= inRotation;
	m_scale		= inScale;
}

std::ostream& Engine::Transform::operator<<(std::ostream& os)
{
	return os << m_position.X() << " " << m_position.Y() << " " << m_position.Z() << " "
			  << m_rotation.W() << " " << m_rotation.X() << " " << m_rotation.Y() << " " << m_rotation.Z() << " "
			  << m_scale.X() << " " << m_scale.Y() << " " << m_scale.Z();
}
