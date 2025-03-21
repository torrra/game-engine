#include "physics/geometry/GeometryTest.h"

#include <iostream>

#include <physx/PxPhysicsAPI.h>

//#include "physics/InternalPhysXStruct.hpp"
//#include "physics/PhysicsEngine.h"

static physx::PxDefaultAllocator		gDefaultAllocatorCallback;
static physx::PxDefaultErrorCallback	gDefaultErrorCallback;

engine::Physics::Physics(void)
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);

	if (m_foundation != nullptr)
	{
		m_pvd = physx::PxCreatePvd(*m_foundation);
	}

	m_transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	if (m_transport != nullptr)
	{
		m_pvd->connect(*m_transport, physx::PxPvdInstrumentationFlag::eALL);
	}

	if (m_foundation != nullptr)
	{
		if (m_pvd != nullptr)
		{
			m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation,
				physx::PxTolerancesScale(), true, m_pvd);
		}
		else
		{
			m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation,
				physx::PxTolerancesScale(), true);
		}
	}

	if (m_physics != nullptr)
	{
		physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

		m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = m_dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		m_scene = m_physics->createScene(sceneDesc);
	}

	m_material = m_physics->createMaterial(0.5f, 0.5f, 0.6f);

	std::cout << "Physics initialized" << std::endl;

	physx::PxRigidStatic* ground = m_physics->createRigidStatic(physx::PxTransform(physx::PxVec3(0, -1, 0), physx::PxQuat(3.14159f / 2, physx::PxVec3(0, 0, 1))));//createRigidStatic(physx::PxTransform(physx::PxVec3(0, -1, 0)));
	physx::PxMaterial* material = m_physics->createMaterial(0.1f, 0.1f, 0.1f);
	physx::PxShape* shape = m_physics->createShape(physx::PxPlaneGeometry(), *material);
	ground->attachShape(*shape);
	m_scene->addActor(*ground);
	shape->release();
}

engine::Physics::~Physics(void)
{
	PX_RELEASE(m_material);
	PX_RELEASE(m_dispatcher);
	PX_RELEASE(m_scene);
	PX_RELEASE(m_physics);
	if (m_pvd != nullptr)
	{
		if (m_transport != nullptr)
		{
			m_transport->disconnect();
			m_pvd->disconnect();
			PX_RELEASE(m_transport);
			PX_RELEASE(m_pvd);
		}
	}
	PX_RELEASE(m_foundation);

	std::cout << "Physics cleaned up" << std::endl;
}

void engine::Physics::StepSimulation(f32 inDeltaTime)
{
	m_scene->simulate(inDeltaTime);
	m_scene->fetchResults(true);
}

physx::PxPhysics* engine::Physics::GetPhysics(void) const
{
	return m_physics;
}

physx::PxScene* engine::Physics::GetScene(void) const
{
	return m_scene;
}

physx::PxMaterial* engine::Physics::GetMaterial(void) const
{
	return m_material;
}

//engine::RigidBody::RigidBody(physx::PxPhysics* inPhysics, physx::PxScene* inScene, physx::PxMaterial* inMaterial, math::Vector3f inPosition/*, f32 inMass*/)
//{
//	//m_rigidDynamicBody = physx::PxCreateDynamic(*inPhysics, physx::PxTransform(ToPxVec3(inPosition), physx::PxQuat(3.14f / 2, physx::PxVec3(0, 0, 1))), physx::PxCapsuleGeometry(1.f, 0.5f), *inMaterial, 1.f);
//
//	//m_rigidDynamicBody = inPhysics->createRigidDynamic(physx::PxTransform(ToPxVec3(inPosition), physx::PxQuat(3.14f / 2, physx::PxVec3(0, 0, 1))));
//	//physx::PxShape* shape = inPhysics->createShape(physx::PxBoxGeometry(1, 1, 1), *inMaterial);
//	//physx::PxShape* shape = inPhysics->createShape(physx::PxSphereGeometry(1), *inMaterial);
//	//physx::PxShape* shape = inPhysics->createShape(physx::PxCapsuleGeometry(0.5f, 1.f), *inMaterial);
//	//shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
//	//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
//	//m_rigidDynamicBody->attachShape(*shape);
//	m_rigidDynamicBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
//	//physx::PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamicBody, inMass);
//	inScene->addActor(*m_rigidDynamicBody);
//	//shape->release();
//
//	physx::PxRigidDynamic* body = inPhysics->createRigidDynamic(physx::PxTransform(physx::PxVec3(10.f, 2.0f, 0.f)));
//	physx::PxMaterial* material = inPhysics->createMaterial(0.f, 0.f, 0.f);
//	physx::PxShape* shape2 = inPhysics->createShape(physx::PxBoxGeometry(1, 1, 1), *material);
//	//body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
//	body->setLinearVelocity(physx::PxVec3(-10, 0, 0));
//	body->attachShape(*shape2);
//	inScene->addActor(*body);
//	shape2->release();
//}

//void engine::RigidBody::SetVelocity(const math::Vector3f& inVelocity)
//{
//	m_rigidDynamicBody->setLinearVelocity(ToPxVec3(inVelocity));
//}

physx::PxRigidDynamic* engine::RigidBody::GetRigidDynamicBody(void) const
{
	return m_rigidDynamicBody;
}

//math::Vector3f engine::RigidBody::GetPosition(void) const
//{
//	return ToVector3(m_rigidDynamicBody->getGlobalPose().p);
//}

//physx::PxVec3 ToPxVec3(const math::Vector3f& inVector)
//{
//	return physx::PxVec3(inVector.GetX(), inVector.GetY(), inVector.GetZ());
//}

//math::Vector3f ToVector3(const physx::PxVec3T<float>& inVector)
//{
//	return math::Vector3f(inVector.x, inVector.y, inVector.z);
//}
