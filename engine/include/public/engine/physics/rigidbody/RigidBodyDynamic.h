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
#include "engine/physics/geometry/Geometry.hpp"
#include "engine/physics/ICollisionListener.h"

#pragma endregion

#pragma region MathLib

#include <math/Vector3.hpp>

#pragma endregion

namespace engine
{
    /// Forward declaration
    struct RigidBodyDynamicImpl;

    class RigidBodyDynamic : public ICollisionListener, public Component
    {
    public :

        using Component::Component;

        /// Constructor
        // Delete the default constructor
                                    RigidBodyDynamic(void) = delete;
        /*
            Initialize the pointer to struct RigidBodyDynamicImpl
            <param> [in] owner : the entity owner
            <param> [in] scene : the scene openGL
        */
        ENGINE_API				    RigidBodyDynamic(EntityHandle inOwner, class SceneGraph* inScene);
        ENGINE_API                  RigidBodyDynamic(RigidBodyDynamic&& inOther) noexcept = default;

        /// Destructor
        // Delete the rigid body and the pointer to struct RigidBodyDynamicImpl
        ENGINE_API				    ~RigidBodyDynamic(void) override {}

        /// Getter
        /*
            Get the gravity status of the rigid body
            <return> [out] the gravity status : true = disabled, false = enabled
        */
        ENGINE_API  bool            IsGravityDisabled(void) const;
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
            Set the gravity status of the rigid body
            <param> [in] inIsGravityDisabled : the gravity status : true = disabled, 
                                                                    false = enabled
        */
        ENGINE_API  void            SetGravityDisabled(bool inIsGravityDisabled);
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

        ENGINE_API void             SetCollisionGroup(collision::ECollisionGroup inCollisionGroup);

        /// Functions
        /*
            Update the entity transform in reference to the dynamic rigid body
            <param> [in] inEntityHandle : the entity
        */
        ENGINE_API	void		    UpdateEntity();
        /*
            Update the dynamic rigid body transform in reference to the entity
            <param> [in] inEntityTransform : the entity
        */
        ENGINE_API	void		    UpdateRigidBody();

        // Delete the dynamic rigid body
        ENGINE_API	void		    RigidBodyDynamicCleanUp(void);

        ENGINE_API	void		    Register(void) override {}

        ENGINE_API void				SerializeText(std::ostream& output,
                                                  EntityHandle owner,
                                                  uint64 index) const override;
        ENGINE_API const char*      DeserializeText(const char* text, const char* end) override;
        ENGINE_API
                    void            SwitchShape(RigidBodyDynamic* inRigidBody, 
                                                const EGeometryType& inGeometry);

        ENGINE_API  void            OnCollisionEnter(void* inOther) override;
        ENGINE_API  void            OnCollisionStay(void* inOther) override { inOther; }
        ENGINE_API  void            OnCollisionExit(void* inOther) override;

        ENGINE_API  void            OnTriggerEnter(void* inOther) override { inOther; }
        ENGINE_API  void            OnTriggerStay(void* inOther) override { inOther; }
        ENGINE_API  void            OnTriggerExit(void* inOther) override { inOther; }

        RigidBodyDynamic&           operator=(RigidBodyDynamic&&) noexcept = default;

        RigidBodyData*              m_data              = nullptr;
        uint64                      m_shape             = 0;
        uint32                      m_type              = EShapeType::DYNAMIC;
        collision::ECollisionGroup  m_collisionGroup    = collision::ECollisionGroup::DEFAULT_COLLISION;


    private :

        friend class RigidBodyDynamicFactory;

        /// Constructor
        // Delete the copy constructor
                                    RigidBodyDynamic(const RigidBodyDynamic& inOther) = delete;


                    void            SetCollisionGroupAndMask(uint32 inCollisionGroup,
                                                             uint32 inCollisionMask);

                    void            SetCapsuleBaseOrientation(void);

        /// Functions
        /*
            Check if the entity has a transform component if not create one
            <return> the entity transform
        */
                    Transform&	    CheckEntityTransform(void);

        // Preset of a box dynamic rigid body
                    void            CreateDynamicBoxRigidBody(void);
        // Preset of a sphere dynamic rigid body
                    void            CreateDynamicSphereRigidBody(void);
        // Preset of a capsule dynamic rigid body
                    void            CreateDynamicCapsuleRigidBody(void);

                    
        /// TODO : Check transform to directly use the component transform
        ///		   of the entity

        /// Private members
        RigidBodyDynamicImpl*	m_rigidBodyImpl = nullptr;
        Material*				m_materialImpl	= nullptr;

    }; // !Class RigidBodyDynamic

    class RigidBodyDynamicFactory
    {
    public :

        /// Functions
        /*
            Fonction to create a dynamic rigid body
            <param> [in] inScene : the scene
            <param> [in] inOwner : the entity owner
            <param> [in] inGeometry : the geometry type
        */
        ENGINE_API static RigidBodyDynamic* CreateDynamic(class SceneGraph* inScene, 
                                                          EntityHandle inOwner,
                                                          const EGeometryType& inGeometry);
    }; // !Class RigidBodyDynamicFactory
} //!Namespace engine