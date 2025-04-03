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

#include "engine/physics/PhysicsMaterial.h"
#include "engine/physics/geometry/Geometry.h"

#pragma endregion

#pragma region MathLib

#include <math/Vector3.hpp>

#pragma endregion

namespace engine
{
    /// Forward declaration
    struct RigidBodyDynamicImpl;

    class RigidBodyDynamic : public Component
    {
    public :

        /// Constructor
        // Delete the default constructor
                                RigidBodyDynamic(void) = delete;
        /*
            Initialize the pointer to struct RigidBodyDynamicImpl
            <param> [in] owner : the entity owner
            <param> [in] scene : the scene openGL
        */
        ENGINE_API				RigidBodyDynamic(EntityHandle owner, class SceneGraph* scene);

        /// Destructor
        // Delete the rigid body and the pointer to struct RigidBodyDynamicImpl
        ENGINE_API				~RigidBodyDynamic(void) override;

        /// Getter
        /*
            Get the gravity status of the rigid body
            <return> [out] the gravity status : true = disabled, false = enabled
        */
        ENGINE_API  bool        GetIsGravityDisabled(void) const;

        /// Setter
        /*
            Set the gravity status of the rigid body
            <param> [in] inIsGravityDisabled : the gravity status : true = disabled, 
                                                                    false = enabled
        */
        ENGINE_API  void        SetGravityDisabled(bool inIsGravityDisabled);

        /// Functions
        // Update the entity transform in reference to the dynamic rigid body
        ENGINE_API	void		UpdateEntity(EntityHandle inEntityHandle);
        // Update the dynamic rigid body transform in reference to the entity
        ENGINE_API	void		UpdateRigidBody(const Transform& inEntityTransform);

        // Delete the dynamic rigid body
        ENGINE_API	void		RigidBodyDynamicCleanUp(void);

        ENGINE_API	void		Register(void) override {}

    private :

        friend class RigidBodyDynamicFactory;

        /// Functions
        // Check if the entity has a transform component if not create one
        // <return> the entity transform
                    Transform&	CheckEntityTransform(void);

        // Preset of a box dynamic rigid body
                    void        CreateDynamicBoxRigidBody(void);
        // Preset of a sphere dynamic rigid body
                    void        CreateDynamicSphereRigidBody(void);
        // Preset of a capsule dynamic rigid body
                    void        CreateDynamicCapsuleRigidBody(void);
                    
        /// TODO : Check transform to directly use the component transform
        ///		   of the entity

        /// Private members
        RigidBodyDynamicImpl*	m_rigidBodyImpl;
        Material*				m_material			= nullptr;

    }; // !Class RigidBodyDynamic

    class RigidBodyDynamicFactory
    {
    public :

        ENGINE_API static RigidBodyDynamic* Create(class SceneGraph* scene, EntityHandle owner,
                                                   const EGeometryType& inGeometry);
    };
} //!Namespace engine