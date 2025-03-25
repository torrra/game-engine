#pragma once

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

#pragma region MathLib

#include <math/Vector3.hpp>
#include <math/VectorGeneric.hpp>
#include <math/Quaternion.hpp>

#pragma endregion

#pragma region Core

#include "engine/core/components/Transform.h"

#pragma endregion

/// Vector
inline physx::PxVec3T<f32> ToPxVec3(const math::Vector3f& inVector)
{
	return physx::PxVec3T<f32>(inVector.GetX(), inVector.GetY(), inVector.GetZ());
}

inline math::Vector3f ToVector3f(const physx::PxVec3T<f32>& inVector)
{
	return math::Vector3f(inVector.x, inVector.y, inVector.z);
}
	
/// Quaternion
inline physx::PxQuat ToPxQuat(const math::Quatf& inQuaternion)
{
	return physx::PxQuat(inQuaternion.GetX(), inQuaternion.GetY(), inQuaternion.GetZ(), inQuaternion.GetW());
}

inline math::Quatf ToQuatf(const physx::PxQuat& inQuaternion)
{
	return math::Quatf(inQuaternion.w, inQuaternion.x, inQuaternion.y, inQuaternion.z);
}

/// Transform
inline physx::PxTransform ToPxTransform(const engine::Transform& inTransform)
{
	return physx::PxTransform(ToPxVec3(inTransform.GetPosition()), ToPxQuat(inTransform.GetRotation()));
}

inline engine::Transform ToTransform(const physx::PxTransform& inTransform)
{
	engine::Transform transform;
	transform.SetPosition(ToVector3f(inTransform.p));
	transform.SetRotation(ToQuatf(inTransform.q));

	return transform;
}