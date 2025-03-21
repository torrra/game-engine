#include "engine/physics/rigidbody/RigidBodyStatic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Standard

#include <iostream>

#pragma endregion

#pragma region Core

#include "engine/core/SceneGraph.h"

#pragma endregion

engine::RigidBodyStatic::RigidBodyStatic(EntityHandle owner, SceneGraph* scene)
{
	m_rigidBodyStaticImpl	= new RigidBodyStaticImpl();
	m_owner					= owner;
	m_currentScene			= scene;
}

engine::RigidBodyStatic::~RigidBodyStatic(void)
{
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

engine::Transform engine::RigidBodyStatic::SetTransform(const Transform& inEntityTransform)
{
	Transform temp;
	temp.SetTransform(inEntityTransform.GetPosition(), math::Quatf(math::Vector3f(0.f, 0.f, 1.f), math::Radian(3.14f / 2)));

	m_rigidBodyStaticImpl->m_rigidBodyStatic->setGlobalPose(ToPxTransform(temp));
	return ToTransform(m_rigidBodyStaticImpl->m_rigidBodyStatic->getGlobalPose());
}

engine::Transform& engine::RigidBodyStatic::CheckEntityTransform(void)
{
	if (Transform* temp = m_currentScene->GetComponent<engine::Transform>(m_owner))
	{
		std::cout << "RigidBodyStatic created, with existing transform" << std::endl;

		return *temp;
	}

	std::cout << "RigidBodyDynamic created, with created transform" << std::endl;

	return *m_currentScene->CreateComponent<engine::Transform>(m_owner);
}
