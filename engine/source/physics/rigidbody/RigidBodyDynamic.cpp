#include "physics/rigidbody/RigidBodyDynamic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Core

#include "core/SceneGraph.h"

#pragma endregion

#include <iostream>

engine::RigidBodyDynamic::RigidBodyDynamic(EntityHandle owner, SceneGraph* scene)
{
	m_rigidBodyImpl = new RigidBodyDynamicImpl();
	m_owner			= owner;
	m_currentScene	= scene;
}

void engine::RigidBodyDynamic::CreateDynamicRigidBody(const PhysicsEngine& inPhysicsEngine,
													  const engine::Material& inMaterial,
													  const Geometry& inGeometry)
{
	/*
		Create a dynamic rigid body
		<param> inPhysicsEngine: The physics engine SDK
		<param> inEntityTransform: The transform of the entity
		<param> PxGeometry: The geometry of the rigid body (box, capsule, sphere, etc.)
		<param> inMaterial: The material of the rigid body
		<param> Density: The density of the rigid body
	*/
	m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
        *inPhysicsEngine.GetImpl().m_physics,
		ToPxTransform(CheckEntityTransform()),
		*inGeometry.GetGeometryImpl().m_geometry,
		*inMaterial.GetImpl().m_material, 1.0f);

    physx::PxTransform currentPose = m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose();
    physx::PxQuat rotation(physx::PxHalfPi, physx::PxVec3(0, 0, 1));

    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        shapes->setLocalPose(physx::PxTransform(physx::PxVec3(0, 0, 0), rotation * currentPose.q));
    }

	// Set the gravity by default
	m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, 
													  m_isGravityDisabled);

	// Add the rigid body to the physics scene
	inPhysicsEngine.GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::CreateDynamicBoxRigidBody(const PhysicsEngine& inPhysicsEngine, 
    const Material& inMaterial, const math::Vector3f& inHalfHeight)
{
    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
        *inPhysicsEngine.GetImpl().m_physics,
        ToPxTransform(CheckEntityTransform()),
        physx::PxBoxGeometry(inHalfHeight.GetX(), inHalfHeight.GetY(), inHalfHeight.GetZ()),
        *inMaterial.GetImpl().m_material, 1.0f);

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        m_isGravityDisabled);

    // Add the rigid body to the physics scene
    inPhysicsEngine.GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::CreateDynamicSphereRigidBody(const PhysicsEngine& inPhysicsEngine, const Material& inMaterial, const f32 inRadius)
{
    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
        *inPhysicsEngine.GetImpl().m_physics,
        ToPxTransform(CheckEntityTransform()),
        physx::PxSphereGeometry(inRadius),
        *inMaterial.GetImpl().m_material, 1.0f);

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        m_isGravityDisabled);

    // Add the rigid body to the physics scene
    inPhysicsEngine.GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::CreateDynamicCapsuleRigidBody(const PhysicsEngine& inPhysicsEngine, const Material& inMaterial, const f32 inRadius, const f32 inHalfHeight)
{
    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
        *inPhysicsEngine.GetImpl().m_physics,
        ToPxTransform(CheckEntityTransform()),
        physx::PxCapsuleGeometry(inRadius, inHalfHeight),
        *inMaterial.GetImpl().m_material, 1.0f);

    physx::PxTransform currentPose = m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose();
    physx::PxQuat rotation(physx::PxHalfPi, physx::PxVec3(0, 0, 1));

    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        shapes->setLocalPose(physx::PxTransform(physx::PxVec3(0, 0, 0), -rotation * currentPose.q));
    }

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        m_isGravityDisabled);

    // Add the rigid body to the physics scene
    inPhysicsEngine.GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::UpdateEntity(EntityHandle inEntityHandle)
{
    //m_rigidBodyImpl->m_rigidBodyDynamic->setLinearVelocity(physx::PxVec3(0.f, -9.81f, 1.f));
    //m_rigidBodyImpl->m_rigidBodyDynamic->setAngularVelocity(physx::PxVec3(0.f, 5.f, 0.f));
	physx::PxTransform transformTemp = m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose();

    *m_currentScene->GetComponent<Transform>(inEntityHandle) = ToTransform(transformTemp);
}

void engine::RigidBodyDynamic::UpdateRigidBody(const Transform& inEntityTransform)
{
	m_rigidBodyImpl->m_rigidBodyDynamic->setGlobalPose(ToPxTransform(inEntityTransform));
}

void engine::RigidBodyDynamic::RigidBodyDynamicCleanUp(void)
{
	PX_RELEASE(m_rigidBodyImpl->m_rigidBodyDynamic);
}

engine::Transform& engine::RigidBodyDynamic::CheckEntityTransform(void)
{
	if (Transform* temp = m_currentScene->GetComponent<engine::Transform>(m_owner))
	{
		return *temp;
	}

	return *m_currentScene->CreateComponent<engine::Transform>(m_owner);
}

engine::RigidBodyDynamic::~RigidBodyDynamic(void)
{
	RigidBodyDynamicCleanUp();

	delete m_rigidBodyImpl;
	m_rigidBodyImpl = nullptr;
}

bool engine::RigidBodyDynamic::GetIsGravityDisabled(void) const
{
    return m_isGravityDisabled;
}

void engine::RigidBodyDynamic::SetGravityDisabled(bool inIsGravityDisabled)
{
    m_isGravityDisabled = inIsGravityDisabled;
}
