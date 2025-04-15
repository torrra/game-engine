#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Core

#include "engine/core/Component.h"
#include "engine/core/components/Transform.h"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsMaterial.h"
#include "engine/physics/geometry/Geometry.hpp"

#pragma endregion

namespace engine
{
	struct RigidBodyStaticImpl;

	class RigidBodyStatic : public Component
	{
	public:

		/// Constructor
		// Default constructor deleted
								    RigidBodyStatic(void) = delete;
        // Constructor with owner and scene parameters
		ENGINE_API				    RigidBodyStatic(EntityHandle inOwner, 
                                                    class SceneGraph* inScene);

		/// Destructor
								    ~RigidBodyStatic(void);
        
        /// Getter
        /*
            Get the half extents of the box
            <return> [out] the half extents
        */
        ENGINE_API  math::Vector3f  GetBoxHalfExtents(void) const;
        /*
            Get the radius of the sphere
            <return> [out] the radius
        */
        ENGINE_API  f32             GetSphereRadius(void) const;
        /*
            Get the half height and radius of the capsule
            <return> [out] the half height and radius
        */
        ENGINE_API  math::Vector2f  GetCapsuleFormat(void) const;
        /// Setter
        /*
            Set the half extents of the box
            <param> [in] inHalfExtents : the half extents
        */
        ENGINE_API  void            SetBoxHalfExtents(math::Vector3f inHalfExtents) const;
        /*
            Set the radius of the sphere
            <param> [in] inRadius : the radius
        */
        ENGINE_API  void            SetSphereRadius(f32 inRadius) const;
        /*
            Set the half height and radius of the capsule
            <param> [in] inRadius : the radius
            <param> [in] inHalfHeight : the half height
        */
        ENGINE_API  void            SetCapsuleFormat(f32 inRadius, f32 inHalfHeight) const;
        /*
            Set the debug draw visualization status
            <param> [in] inIsDebugVisualization : the visualization status : true = enabled
                                                                             false = disabled
        */
        ENGINE_API  void            SetDebugVisualization(bool inIsDebugVisualization);

		/// Functions
		ENGINE_API	void		    Register(void) override {}

		// Delete the dynamic rigid body resources
		ENGINE_API	void		    RigidBodyStaticCleanUp(void);

	private :

        friend class RigidBodyStaticFactory;

		/// Functions
        /*
            Check if the entity has a transform component if not create one
            <return> the entity transform
        */
					Transform&	    CheckEntityTransform(void);

                    // Preset of a box dynamic rigid body
                    void            CreateStaticBoxRigidBody(void);
                    // Preset of a sphere dynamic rigid body
                    void            CreateStaticSphereRigidBody(void);
                    // Preset of a capsule dynamic rigid body
                    void            CreateStaticCapsuleRigidBody(void);
                    // Preset of a plane static rigid body
                    void            CreateStaticPlaneRigidBody(void);

		RigidBodyStaticImpl*    m_rigidBodyStaticImpl   = nullptr;
        Material*               m_materialImpl          = nullptr;

	}; // !Class RigidBodyStatic

    class RigidBodyStaticFactory
    {
    public:

        /// Functions
        /*
            Fonction to create a dynamic rigid body
            <param> [in] inScene : the scene
            <param> [in] inOwner : the entity owner
            <param> [in] inGeometry : the geometry type
        */
        ENGINE_API static RigidBodyStatic* CreateStatic(class SceneGraph* inScene, EntityHandle inOwner,
                                                        const EGeometryType& inGeometry);
    };
} // !Namespace engine