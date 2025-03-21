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

engine::RigidBodyDynamic::RigidBodyDynamic(EntityHandle owner, SceneGraph* scene)
{
	m_rigidBodyImpl = new RigidBodyDynamicImpl();
	m_owner			= owner;
	m_currentScene	= scene;
}

void engine::RigidBodyDynamic::CreateDynamicRigidBody(const PhysicsEngine& inPhysicsEngine,
													  const engine::Transform& inEntityTransform,
													  const engine::Material& inMaterial)
{
	/*
		Create a dynamic rigid body
		<param> inPhysicsEngine: The physics engine SDK
		<param> inEntityTransform: The transform of the entity
		<param> PxGeometry: The geometry of the rigid body (box, capsule, sphere, etc.)
		<param> inMaterial: The material of the rigid body
		<param> Density: The density of the rigid body
	*/
	m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(*inPhysicsEngine.GetImpl().m_physics,
										  ToPxTransform(inEntityTransform),
										  physx::PxSphereGeometry(0.5f), *inMaterial.GetImpl().m_material, 1.0f);

	// Update the rigid body transform rotation to be in the correct orientation
	m_rigidBodyImpl->m_rigidBodyDynamic->setGlobalPose(ToPxTransform(SetTransform(inEntityTransform)));

	// Set the gravity by default
	m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, 
													  m_isGravityDisabled);

	// Add the rigid body to the physics scene
	inPhysicsEngine.GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::UpdateEntity(void)
{
	physx::PxTransform transformTemp = m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose();

	m_currentScene->UpdateComponents<engine::Transform>(ToTransform(transformTemp));
}

void engine::RigidBodyDynamic::UpdateRigidBody(const Transform& inEntityTransform)
{
	m_rigidBodyImpl->m_rigidBodyDynamic->setGlobalPose(ToPxTransform(inEntityTransform));
}

void engine::RigidBodyDynamic::RigidBodyDynamicCleanUp(void)
{
	PX_RELEASE(m_rigidBodyImpl->m_rigidBodyDynamic);
}

engine::Transform engine::RigidBodyDynamic::SetTransform(const Transform& inEntityTransform)
{
	Transform temp;
	temp.SetTransform(inEntityTransform.GetPosition(), math::Quatf(math::Vector3f(0.f, 0.f, 1.f), math::Radian(3.14f / 2)));

	m_rigidBodyImpl->m_rigidBodyDynamic->setGlobalPose(ToPxTransform(temp));
	return ToTransform(m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose());
}

engine::RigidBodyDynamic::~RigidBodyDynamic(void)
{
	RigidBodyDynamicCleanUp();

	delete m_rigidBodyImpl;
	m_rigidBodyImpl = nullptr;
}
