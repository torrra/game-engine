#include "physics/PhysicsEngine.h"

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

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

ENGINE_API engine::PhysicsEngine& engine::PhysicsEngine::Get(void)
{
	if (m_instance == nullptr)
	{
		m_instance = new engine::PhysicsEngine();
	}

	return *m_instance;
}