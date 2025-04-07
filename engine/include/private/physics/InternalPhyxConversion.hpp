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
// Convert from math::Vector3f to physx::PxVec3
inline physx::PxVec3T<f32> ToPxVec3(const math::Vector3f& inVector)
{
	return physx::PxVec3T<f32>(inVector.GetX(), inVector.GetY(), inVector.GetZ());
}
// Convert from physx::PxVec3 to math::Vector3f
inline math::Vector3f ToVector3f(const physx::PxVec3T<f32>& inVector)
{
	return math::Vector3f(inVector.x, inVector.y, inVector.z);
}
// Convert from physx::PxDebugLine pos 0 to math::Vector3f
inline math::Vector3f ToVector3fPos0(const physx::PxDebugLine& inVector)
{
    return math::Vector3f(inVector.pos0.x, inVector.pos0.y, inVector.pos0.z);
}
// Convert from physx::PxDebugLine pos 1 to math::Vector3f
inline math::Vector3f ToVector3fPos1(const physx::PxDebugLine& inVector)
{
    return math::Vector3f(inVector.pos1.x, inVector.pos1.y, inVector.pos1.z);
}
	
/// Quaternion
// Convert from math::Quatf to physx::PxQuat
inline physx::PxQuat ToPxQuat(const math::Quatf& inQuaternion)
{
	return physx::PxQuat(inQuaternion.GetX(), inQuaternion.GetY(), inQuaternion.GetZ(), inQuaternion.GetW());
}
// Convert from physx::PxQuat to math::Quatf
inline math::Quatf ToQuatf(const physx::PxQuat& inQuaternion)
{
	return math::Quatf(inQuaternion.w, inQuaternion.x, inQuaternion.y, inQuaternion.z);
}

/// Transform
// Convert from engine::Transform to physx::PxTransform
inline physx::PxTransform ToPxTransform(const engine::Transform& inTransform)
{
	return physx::PxTransform(ToPxVec3(inTransform.GetPosition()), ToPxQuat(inTransform.GetRotation()));
}
// Convert from physx::PxTransform to engine::Transform
inline engine::Transform ToTransform(const physx::PxTransform& inTransform)
{
	engine::Transform transform;
	transform.SetPosition(ToVector3f(inTransform.p));
	transform.SetRotation(ToQuatf(inTransform.q));

	return transform;
}