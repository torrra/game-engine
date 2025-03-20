#include "engine/physics/rigidbody/RigidBodyDynamic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"

#pragma endregion

engine::RigidBodyDynamic::RigidBodyDynamic(void)
{
	m_rigidBodyImpl = new RigidBodyDynamicImpl();
}

void engine::RigidBodyDynamic::CreateDynamicRigidBody(const PhysicsEngine& inPhysicsEngine)
{
	physx::PxMaterial* material = inPhysicsEngine.GetImpl().m_physics->createMaterial(0.f, 0.f, 0.f);
	m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(*inPhysicsEngine.GetImpl().m_physics,
										  physx::PxTransform(physx::PxVec3(-10.f, 2.f, 0.f),
											  physx::PxQuat(3.14f / 2, physx::PxVec3(0, 0, 1))),
										  physx::PxCapsuleGeometry(0.5f, 1.f), *material, 1.0f);

	m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, 
													  m_isGravityDisabled);

	inPhysicsEngine.GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::RigidBodyDynamicCleanUp(void)
{
	PX_RELEASE(m_rigidBodyImpl->m_rigidBodyDynamic);
}

engine::RigidBodyDynamic::~RigidBodyDynamic(void)
{
	RigidBodyDynamicCleanUp();

	delete m_rigidBodyImpl;
	m_rigidBodyImpl = nullptr;
}
