#include "physics/rigidbody/RigidBodyDynamic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Core

#include "core/SceneGraph.h"

#pragma endregion

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
	m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(*inPhysicsEngine.GetImpl().m_physics,
		ToPxTransform(CheckEntityTransform()),
		*inGeometry.GetGeometryImpl().m_geometry,
		*inMaterial.GetImpl().m_material, 1.0f);

	// Set the gravity by default
	m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, 
													  m_isGravityDisabled);

	// Add the rigid body to the physics scene
	inPhysicsEngine.GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::UpdateEntity(EntityHandle inEntityHandle)
{
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

engine::Transform engine::RigidBodyDynamic::SetTransform(const Transform& inEntityTransform)
{
	m_rigidBodyImpl->m_rigidBodyDynamic->setGlobalPose(ToPxTransform(inEntityTransform));
	return ToTransform(m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose());
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
