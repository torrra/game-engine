#pragma once

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

namespace engine
{
	// Create a structure of physX elements to store them
	struct PhysicsEngineImpl
	{
		physx::PxFoundation*			m_foundation		= nullptr;
		physx::PxPhysics*				m_physics			= nullptr;
		physx::PxScene*					m_scene				= nullptr;
		physx::PxDefaultCpuDispatcher*	m_dispatcher		= nullptr;
		physx::PxMaterial*				m_material			= nullptr;
		physx::PxPvd*					m_pvd				= nullptr;

	}; // !Struct PhysicsEngineImpl

	struct RigidBodyDynamicImpl
	{
		physx::PxRigidDynamic*			m_rigidBodyDynamic	= nullptr;

	}; // !Struct RigidBodyImpl
} // !Namespace engine