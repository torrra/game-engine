#include "engine/physics/rigidbody/RigidBodyStatic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Core

#include "core/SceneGraph.h"

#pragma endregion

engine::RigidBodyStatic::RigidBodyStatic(EntityHandle owner, SceneGraph* scene)
{
	m_rigidBodyStaticImpl	= new RigidBodyStaticImpl();
	m_owner					= owner;
	m_currentScene			= scene;
}

engine::RigidBodyStatic::~RigidBodyStatic(void)
{
	RigidBodyStaticCleanUp();

	delete m_rigidBodyStaticImpl;
	m_rigidBodyStaticImpl = nullptr;
}

void engine::RigidBodyStatic::CreateStaticRigidBody(const PhysicsEngine& inPhysicsEngine, 
	const Material& inMaterial, const Geometry& inGeometry)
{
	m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreateStatic(*inPhysicsEngine.GetImpl().m_physics,
		ToPxTransform(CheckEntityTransform()),
		*inGeometry.GetGeometryImpl().m_geometry,
		*inMaterial.GetImpl().m_material);

	// Update the rigid body transform rotation to be in the correct orientation
	m_rigidBodyStaticImpl->m_rigidBodyStatic->setGlobalPose(ToPxTransform(SetTransform(
		*m_currentScene->GetComponent<engine::Transform>(m_owner))));

	// Add the rigid body to the physics scene
	inPhysicsEngine.GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
}

void engine::RigidBodyStatic::RigidBodyStaticCleanUp(void)
{
	PX_RELEASE(m_rigidBodyStaticImpl->m_rigidBodyStatic);
}

engine::Transform engine::RigidBodyStatic::SetTransform(const Transform& inEntityTransform)
{
	m_rigidBodyStaticImpl->m_rigidBodyStatic->setGlobalPose(ToPxTransform(inEntityTransform));
	return ToTransform(m_rigidBodyStaticImpl->m_rigidBodyStatic->getGlobalPose());
}

engine::Transform& engine::RigidBodyStatic::CheckEntityTransform(void)
{
	if (Transform* temp = m_currentScene->GetComponent<engine::Transform>(m_owner))
	{
		return *temp;
	}

	return *m_currentScene->CreateComponent<engine::Transform>(m_owner);
}
