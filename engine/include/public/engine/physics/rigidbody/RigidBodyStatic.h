#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Core

#include "engine/core/Component.h"
#include "engine/core/components/Transform.h"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/Material.h"
#include "engine/physics/geometry/Geometry.h"

#pragma endregion

namespace engine
{
	struct RigidBodyStaticImpl;

	class RigidBodyStatic : Component
	{
	public:

		/// Constructor
		// Default constructor deleted
								RigidBodyStatic(void) = delete;
		ENGINE_API				RigidBodyStatic(EntityHandle owner, class SceneGraph* scene);

		/// Destructor
								~RigidBodyStatic(void);

		/// Functions
		ENGINE_API	void		CreateStaticRigidBody(const PhysicsEngine& inPhysicsEngine,
													  const Material& inMaterial,
													  const Geometry& inGeometry);

		ENGINE_API	void		Register(void) override {}

	private :

		/// Setter
		// Set the transform rotation of the rigidbody by default in reference 
		// to the entity transform
					Transform	SetTransform(const Transform& inEntityTransform);

		/// Functions
		// Check if the entity has a transform component if not create one
		// <return> the entity transform
					Transform&	CheckEntityTransform(void);

		RigidBodyStaticImpl* m_rigidBodyStaticImpl = nullptr;

	}; // !Class RigidBodyStatic
} // !Namespace engine