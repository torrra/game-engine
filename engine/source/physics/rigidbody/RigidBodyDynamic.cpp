#include "engine/physics/rigidbody/RigidBodyDynamic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Core

#include "engine/core/SceneGraph.h"

#pragma endregion

#pragma region Standard

#include <iostream>

#pragma endregion

//engine::RigidBodyDynamic::RigidBodyDynamic(void)
//{
//	m_rigidBodyImpl = new RigidBodyDynamicImpl();
//}

engine::RigidBodyDynamic::RigidBodyDynamic(EntityHandle owner, SceneGraph* scene)
{
	m_rigidBodyImpl = new RigidBodyDynamicImpl();
	m_owner			= owner;
	m_currentScene	= scene;
}

void engine::RigidBodyDynamic::CreateDynamicRigidBody(const PhysicsEngine& inPhysicsEngine,
													  const engine::Transform& inEntityTransform)
{
	physx::PxMaterial* material = inPhysicsEngine.GetImpl().m_physics->createMaterial(0.f, 0.f, 0.f);
	m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(*inPhysicsEngine.GetImpl().m_physics,
										  ToPxTransform(inEntityTransform),
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
