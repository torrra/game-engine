#pragma once

#pragma region Engine

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#pragma endregion

#pragma region Core

#include "engine/core/components/Transform.h"
#include "engine/core/Component.h"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/Material.h"
#include "engine/physics/geometry/Geometry.h"

#pragma endregion

#pragma region MathLib

#include <math/Vector3.hpp>

#pragma endregion

namespace engine
{
	struct RigidBodyDynamicImpl;

	class RigidBodyDynamic : public Component
	{
	public :

		/// Constructor
		// Delete the default constructor
								RigidBodyDynamic(void) = delete;
		// Initialize the pointer to struct RigidBodyDynamicImpl
		ENGINE_API				RigidBodyDynamic(EntityHandle owner, class SceneGraph* scene);

		/// Destructor
		// Delete the rigid body and the pointer to struct RigidBodyDynamicImpl
		ENGINE_API				~RigidBodyDynamic(void) override;

        /// Getter
        // Get the gravity status of the rigid body
        // <return> the gravity status : true = disabled, false = enabled
        ENGINE_API  bool        GetIsGravityDisabled(void) const;

        /// Setter
        // Set the gravity status of the rigid body
        // <param> inIsGravityDisabled : the gravity status : true = disabled, false = enabled
        ENGINE_API  void        SetGravityDisabled(bool inIsGravityDisabled);

		/// Functions
		// Create a dynamic rigid body with default values
		// <param> inPhysicsEngine : the physics engine
		ENGINE_API	void		CreateDynamicRigidBody(const PhysicsEngine& inPhysicsEngine,
													   const Material& inMaterial,
													   const Geometry& inGeometry);
        ENGINE_API  void        CreateDynamicBoxRigidBody(const PhysicsEngine& inPhysicsEngine,
                                                          const Material& inMaterial,
                                                          const math::Vector3f& inHalfHeight);
        ENGINE_API  void        CreateDynamicSphereRigidBody(const PhysicsEngine& inPhysicsEngine,
                                                             const Material& inMaterial,
                                                             const f32 inRadius);
        ENGINE_API  void        CreateDynamicCapsuleRigidBody(const PhysicsEngine& inPhysicsEngine,
                                                               const Material& inMaterial,
                                                               const f32 inRadius,
                                                               const f32 inHalfHeight);
		// Update the entity transform in reference to the dynamic rigid body
		ENGINE_API	void		UpdateEntity(EntityHandle inEntityHandle);
		// Update the dynamic rigid body transform in reference to the entity
		ENGINE_API	void		UpdateRigidBody(const Transform& inEntityTransform);

		// Delete the dynamic rigid body
		ENGINE_API	void		RigidBodyDynamicCleanUp(void);

		ENGINE_API	void		Register(void) override {}

	private :

		/// Functions
		// Check if the entity has a transform component if not create one
		// <return> the entity transform
					Transform&	CheckEntityTransform(void);
                    
		/// TODO : Check transform to directly use the component transform
		///		   of the entity

		/// Private members
		RigidBodyDynamicImpl*	m_rigidBodyImpl;
		PhysicsEngine*			m_physicsEngine		= nullptr;
		Material*				m_material			= nullptr;
        bool					m_isGravityDisabled = false;

	}; // !Class RigidBodyDynamic
} //!Namespace engine