#pragma once

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

#pragma region Standard

#include <vector>

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

	}; // !Struct RigidBodyDynamicImpl

	struct RigidBodyStaticImpl
	{
		physx::PxRigidStatic*			m_rigidBodyStatic	= nullptr;

	}; // !Struct RigidBodyStaticImpl

	struct MaterialImpl
	{
		physx::PxMaterial*				m_material			= nullptr;

	}; // !Struct MaterialImpl

    struct DebugDrawImpl
    {
        const physx::PxRenderBuffer*    m_renderBuffer      = nullptr;
        std::vector<physx::PxDebugLine> m_customLines       = {};

    }; // !Struct DebugDrawImpl

    struct RaycastImpl
    {
        physx::PxRaycastBuffer*         m_hit               = nullptr;
        physx::PxQueryFilterData        m_queryFilterData;
        physx::PxDebugLine*             m_successLine       = nullptr;
        physx::PxDebugLine*             m_failureLine       = nullptr;

    }; // !Struct RaycastImpl

    /// PhysX using
    using PxVisualParam = physx::PxVisualizationParameter;
    using PxInstruFlag  = physx::PxPvdInstrumentationFlag;
    using PxTolerances  = physx::PxTolerancesScale;

} // !Namespace engine
