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

#pragma region MathLib

#include <math/VectorGeneric.hpp>
#include <math/Vector3.hpp>

#pragma endregion

#pragma region Core

#include "engine/core/Component.h"

#pragma endregion

namespace engine
{
	struct RigidBodyDynamicImpl;

	class RigidBodyDynamic : Component
	{
	public :

		/// Constructor
		// Initialize the pointer to struct RigidBodyDynamicImpl
		//ENGINE_API		RigidBodyDynamic(void);
		ENGINE_API		RigidBodyDynamic(EntityHandle owner, class SceneGraph* scene);

		/// Destructor
		// Delete the rigid body and the pointer to struct RigidBodyDynamicImpl
		ENGINE_API		~RigidBodyDynamic(void) override;

		/// Functions
		// Create a dynamic rigid body with default values
		// <param> inPhysicsEngine : the physics engine
		ENGINE_API void CreateDynamicRigidBody(const PhysicsEngine& inPhysicsEngine, 
											   const engine::Transform& inEntityTransform);
		// Delete the dynamic rigid body
		ENGINE_API void RigidBodyDynamicCleanUp(void);

		ENGINE_API void Register(void) override {}

	private :

		/// TODO : Check transform to directly use the component transform
		///		   of the entity

		/// Private members
		RigidBodyDynamicImpl*	m_rigidBodyImpl;
		PhysicsEngine*			m_physicsEngine		= nullptr;
		bool					m_isGravityDisabled = true;

	}; // !Class RigidBodyDynamic
} //!Namespace engine