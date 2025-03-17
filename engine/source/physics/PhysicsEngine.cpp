#include "physics/PhysicsEngine.h"

#pragma region Standard

#include <iostream>

#pragma endregion

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

physx::PxDefaultAllocator		gDefaultAllocatorCallback;
physx::PxDefaultErrorCallback	gDefaultErrorCallback;

engine::PhysicsEngine* engine::PhysicsEngine::m_instance = nullptr;

struct engine::PhysicsEngineImpl 
{
	physx::PxFoundation*			m_foundation	= nullptr;
	physx::PxPhysics*				m_physics		= nullptr;
	physx::PxScene*					m_scene			= nullptr;
	physx::PxDefaultCpuDispatcher*	m_dispatcher	= nullptr;
	physx::PxMaterial*				m_material		= nullptr;
	physx::PxPvd*					m_pvd			= nullptr;

}; // !Struct PhysicsEngineImpl

engine::PhysicsEngine::PhysicsEngine()
{
	m_impl = new PhysicsEngineImpl();
}

engine::PhysicsEngine::~PhysicsEngine(void)
{
	delete m_impl;
}

engine::PhysicsEngine& engine::PhysicsEngine::Get(void)
{
	if (m_instance == nullptr)
	{
		m_instance = new engine::PhysicsEngine();
	}

	return *m_instance;
}

void engine::PhysicsEngine::Init(void)
{
	PhysicsEngineImpl& impl = *m_impl;

	/// TODO : Delete debug cout
	(impl.m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback)) != NULL ?
		std::cout << "Foundation created" << std::endl :
		std::cout << "Failed to create foundation" << std::endl;

	(impl.m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *impl.m_foundation, 
									  physx::PxTolerancesScale(), true)) != NULL ?
		std::cout << "Physics created" << std::endl :
		std::cout << "Failed to create physics" << std::endl;

	physx::PxPvdTransport* transport;
	(transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10)) != NULL ?
		std::cout << "Transport created" << std::endl :
		std::cout << "Failed to create transport" << std::endl;

	(impl.m_pvd = physx::PxCreatePvd(*impl.m_foundation)) != NULL ?
		std::cout << "Pvd created" << std::endl :
		std::cout << "Failed to create pvd" << std::endl;

	(impl.m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL)) ?
		std::cout << "Pvd connected" << std::endl :
		std::cout << "Failed to connect pvd" << std::endl;

	transport->release();

	physx::PxSceneDesc sceneDesc(impl.m_physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	impl.m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = impl.m_dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	impl.m_scene = impl.m_physics->createScene(sceneDesc);
	impl.m_material = impl.m_physics->createMaterial(0.5f, 0.5f, 0.6f);
}

void engine::PhysicsEngine::CleanUp(void)
{
	PhysicsEngineImpl& impl = *m_impl;
	PX_RELEASE(impl.m_material);
	std::cout << "Material released" << std::endl;
	PX_RELEASE(impl.m_scene);
	std::cout << "Scene released" << std::endl;
	PX_RELEASE(impl.m_dispatcher);
	std::cout << "Dispatcher released" << std::endl;
	PX_RELEASE(impl.m_pvd);
	std::cout << "Pvd released" << std::endl;
	PX_RELEASE(impl.m_physics);
	std::cout << "Physics released" << std::endl;
	PX_RELEASE(impl.m_foundation);
	std::cout << "Foundation released" << std::endl;

	delete m_instance;
	m_instance = nullptr;
}