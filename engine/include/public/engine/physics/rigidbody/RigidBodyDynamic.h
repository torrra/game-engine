#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Core

#include "engine/core/components/Transform.h"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"

#pragma endregion

namespace engine
{
	struct RigidBodyDynamicImpl;

	class RigidBodyDynamic
	{
	public :

		/// Constructor
		// Initialize the pointer to struct RigidBodyDynamicImpl
		ENGINE_API		RigidBodyDynamic(void);

		/// Destructor
		// Delete the rigid body and the pointer to struct RigidBodyDynamicImpl
		ENGINE_API		~RigidBodyDynamic(void);

		/// Functions
		// Create a dynamic rigid body with default values
		// <param> inPhysicsEngine : the physics engine
		ENGINE_API void CreateDynamicRigidBody(const PhysicsEngine& inPhysicsEngine);
		// Delete the dynamic rigid body
		ENGINE_API void RigidBodyDynamicCleanUp(void);

	private :

		/// Private members
		RigidBodyDynamicImpl*	m_rigidBodyImpl;
		bool					m_isGravityDisabled = false;

	}; // !Class RigidBodyDynamic
} //!Namespace engine