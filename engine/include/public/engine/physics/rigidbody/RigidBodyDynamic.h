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
#include <math/Vector2.hpp>

#pragma endregion

namespace engine
{
    /// Forward declaration
    struct RigidBodyDynamicImpl;

    class RigidBodyDynamic : public ICollisionListener, public Component
    {
    public :

        /// Constructor        
        /*
            Initialize the pointer to struct RigidBodyDynamicImpl
            <param> [in] owner : the entity owner
            <param> [in] scene : the scene openGL
        */
        ENGINE_API			    RigidBodyDynamic(EntityHandle inOwner, class SceneGraph* inScene);
        // Default copy assignement
        ENGINE_API              RigidBodyDynamic(RigidBodyDynamic&& inOther) noexcept = default;

        /// Destructor
        // Delete the rigid body and the pointer to struct RigidBodyDynamicImpl
        ENGINE_API			    ~RigidBodyDynamic(void) override {}

        /// Operator
        // Default move assignement
        ENGINE_API 
            RigidBodyDynamic&   operator=(RigidBodyDynamic&& inOther) noexcept = default;

        /// Getter
        /*
            Get the gravity status of the rigid body
            <return> [out] the gravity status : true    = disabled
                                                false   = enabled
        */
        ENGINE_API [[nodiscard]]
            bool                IsGravityDisabled(void) const;
        /*
            Get the half extents of the box
            <return> [out] the half extents
        */
        ENGINE_API [[nodiscard]]
            math::Vector3f      GetBoxHalfExtents(void);
        /*
            Get the radius of the sphere
            <return> [out] the radius
        */
        ENGINE_API [[nodiscard]]
            f32                 GetSphereRadius(void);
        /*
            Get the half height and radius of the capsule
            <return> [out] the half height and radius
        */
        ENGINE_API [[nodiscard]]
            math::Vector2f      GetCapsuleFormat(void);
        /*
            Get the linear velocity of the rigidbody
            <return> [out] Vector3 : linear velocity
        */
        ENGINE_API [[nodiscard]]
            math::Vector3f      GetLinearVelocity(void) const;
        /*
            Get the angular velocity
            <return> [out] Vector3 : angular velocity
        */
        ENGINE_API [[nodiscard]]
            math::Vector3f      GetAngularVelocity(void) const;
        /*
            Get if the rigidbody can be trigger
            <return> [out] bool :   true    = can be trigger
                                    false   = can not be trigger
        */
        ENGINE_API [[nodiscard]]
            bool                GetIsTrigger(void) const;
        /*
            Get if the rigidbody rotation is lock around X axis
            <return> [out] bool :   true    = x axis rotation locked
                                    false   = x axis rotation unlocked
        */
        ENGINE_API [[nodiscard]]
            bool                GetIsXAxisLock(void) const;
        /*
            Get if the rigidbody rotation is lock around Y axis
            <return> [out] bool :   true    = y axis rotation locked
                                    false   = y axis rotation unlocked
        */
        ENGINE_API [[nodiscard]]
            bool                GetIsYAxisLock(void) const;
        /*
            Get if the rigidbody rotation is lock around Z axis
            <return> [out] bool :   true    = z axis rotation locked
                                    false   = z axis rotation unlocked
        */
        ENGINE_API [[nodiscard]]
            bool                GetIsZAxisLock(void) const;

        /// Setter
        /*
            Set the gravity status of the rigid body
            <param> [in] inIsGravityDisabled : the gravity status : true    = disabled, 
                                                                    false   = enabled
        */
        ENGINE_API  
            void                SetGravityDisabled(bool inIsGravityDisabled);
        /*
            Set the half extents of the box
            <param> [in] inHalfExtents : the half extents
        */
        ENGINE_API  
            void                SetBoxHalfExtents(const math::Vector3f& inHalfExtents);
        /*
            Set the radius of the sphere
            <param> [in] inRadius : the radius
        */
        ENGINE_API  
            void                SetSphereRadius(f32 inRadius);
        /*
            Set the half height and radius of the capsule
            <param> [in] inRadius       : the radius
            <param> [in] inHalfHeight   : the half height
        */
        ENGINE_API  
            void                SetCapsuleFormat(f32 inRadius, f32 inHalfHeight);
        /*
            Set the debug draw visualization status
            <param> [in] inIsDebugVisualization : the visualization status : true   = enabled
                                                                             false  = disabled
        */
        ENGINE_API  
            void                SetDebugVisualization(bool inIsDebugVisualization);
        /*
            Set the collision group (cf : ICollisionListener.h enum ECollisionGroup)
            (Only one per one)
            <param> [in] collision group :  - NONE_COLLISION
                                            - DEFAULT_COLLISION
                                            - PROJECTILE_COLLISION
                                            - ENVIRONMENT_COLLISION
                                            - TRIGGER_COLLISION
        */
        ENGINE_API  
            void                SetCollisionGroup(collision::ECollisionGroup inCollisionGroup);

        /*
            Set if the rigidbody can be trigger
            <param> [in] bool : true    = Rigidbody can be trigger
                                false   = Rigidbody can not be trigger
        */
        ENGINE_API  
            void                SetTrigger(bool inIsTrigger);

        /*
            Set the linear velocity of the rigidbody
            <param> [in] Vector3            : Linear velocity
            <param> [in] bool (optional)    : Wake the object if it is asleep
        */
        ENGINE_API  
            void                SetLinearVelocity(const math::Vector3f& inLinearVelocity, 
                                                  bool inAutoWake = true);
        /*
            Set the angular velocity of the rigidbody
            <param> [in] Vector3            : Angular velocity
            <param> [in] bool (optional)    : Wake the object if it is asleep
        */
        ENGINE_API  
            void                SetAngularVelocity(const math::Vector3f& inAngularVelocity, 
                                                   bool inAutoWake = true);
        /*
            Set the rotation lock or unlock around X axis
            <param> [in] bool :     true    = Rotation locked
                                    false   = Rotation unlocked
        */
        ENGINE_API  
            void                SetXAxisLock(bool inAxisLock);
        /*
            Set the rotation lock or unlock around Y axis
            <param> [in] bool :     true    = Rotation locked
                                    false   = Rotation unlocked
        */
        ENGINE_API  
            void                SetYAxisLock(bool inAxisLock);
        /*
            Set the rotation lock or unlock around Z axis
            <param> [in] bool :     true    = Rotation locked
                                    false   = Rotation unlocked
        */
        ENGINE_API  
            void                SetZAxisLock(bool inAxisLock);

        /// Functions
        // Update the entity transform in reference to the dynamic rigid body
        ENGINE_API	
            void		        UpdateEntity(void);
        // Update the dynamic rigid body transform in reference to the entity
        ENGINE_API	
            void		        UpdateRigidBody(void);
        // Delete the dynamic rigid body
        ENGINE_API	
            void		        RigidBodyDynamicCleanUp(void);
        ENGINE_API	
            void		        Register(void) override {}
        /*
            Serialize rigidbody's member
            <param> [in] output : output stream
            <param> [in] owner  : Owner of the rigidbody component
            <param> [in] index  : Index of this component in an array of rigidbody component
        */
        ENGINE_API  
            void			    SerializeText(std::ostream& output,
                                              EntityHandle owner,
                                              uint64 index) const override;
        /*
            Deserialize rigidibody's member
            <param> [in] text   : The text to deserialize
            <param> [in] end    : The end of the file
        */
        ENGINE_API  
            const char*         DeserializeText(const char* text, const char* end) override;
        /*
            Change the shape of the rigidbody by creating a new one
            <param> [in] Geometry type :    - BOX
                                            - SPHERE
                                            - CAPSULE
            <warning> If Plane selected, set a box by default, plane doesn't exists in dynamic
        */
        ENGINE_API  
            void                SwitchShape(const EGeometryType& inGeometry);
        /*
            Check the collision at the begining
            <param> [in] inOther : The other entity which collides with this
        */
        ENGINE_API  
            void                OnCollisionEnter(EntityHandle inOther) override;

        ENGINE_API [[maybe_unused]]
            void                OnCollisionStay([[maybe_unused]] EntityHandle inOther) override {};
        /*
            Check the collision at the end
            <param> [in] inOther : The other entity which collides with this
        */
        ENGINE_API  
            void                OnCollisionExit(EntityHandle inOther) override;
        /*
            Check the trigger collision at the begining
            <param> [in] inOther : The other entity which enter in this trigger
        */
        ENGINE_API  
            void                OnTriggerEnter(EntityHandle inOther) override;
        
        ENGINE_API [[maybe_unused]] [[deprecated]]
            void                OnTriggerStay([[maybe_unused]] EntityHandle inOther) override {};
        /*
            Check the trigger collision at the end
            <param> [in] inOther : The other entity which enter in this trigger
        */
        ENGINE_API  
            void                OnTriggerExit(EntityHandle inOther) override;
        /*
            Add force to this rigidbody
            <param> [in] inForce        : Vector3       : the force to add to the rigidbody
            <param> [in] inForceMode    : EForceMode    :   - FORCE
                                                            - IMPULSE
                                                            - VELOCITY_CHANGE
                                                            - ACCELERATION
            (optional)(Can accumulate multiple force mode)
            <param> [in] inAutoWake     : bool          : Wake the object if it is asleep (optional)
        */
        ENGINE_API  
            void                AddForce(const math::Vector3f& inForce, 
                                         EForceMode inForceMode = EForceMode::FORCE, 
                                         bool inAutoWake = true);
        /*
            Add torque to this rigidbody
            <param> [in] inTorque       : Vector3       : the torque to add to the rigidbody
            <param> [in] inForceMode    : EForceMode    :   - FORCE
                                                            - IMPULSE
                                                            - VELOCITY_CHANGE
                                                            - ACCELERATION
            (optional)(Can accumulate multiple force mode)
            <param> [in] inAutoWake     : bool          : Wake the object if it is asleep (optional)
        */
        ENGINE_API  
            void                AddTorque(const math::Vector3f& inTorque, 
                                          EForceMode inForceMode = EForceMode::FORCE, 
                                          bool inAutoWake = true);

        // RigidBody shape { Box, Sphere, Capsule }
        uint64                          m_rigidBodyShape    = 0;
        // Struct to store index and rigidBody type
        RigidBodyData                   m_data;
        // Rigidbody type { Dynamic, Static }
        uint32                          m_rigidBodyType     = EShapeType::DYNAMIC;
        // Collision group { None, Default, Projectile, Environment, Trigger }
        collision::ECollisionGroup      m_collisionGroup    = 
                                                    collision::ECollisionGroup::DEFAULT_COLLISION;


    private :

        /// Friend
        friend class RigidBodyDynamicFactory;

        /// Constructor
        // Delete the default constructor
                                    RigidBodyDynamic(void) = delete;
        // Delete the copy constructor
                                    RigidBodyDynamic(const RigidBodyDynamic& inOther) = delete;

        /// Operator
        // Copy assignement deleted
            RigidBodyDynamic&       operator=(const RigidBodyDynamic& inOther) = delete;

        /// Setter
        /*
            Set the group and mask collision to determine with which object this will collide
            <param> [in] inCollisionGroup : - NONE_COLLISION
                                            - DEFAULT_COLLISION
                                            - PROJECTILE_COLLISION
                                            - ENVIRONMENT_COLLISION
                                            - TRIGGER_COLLISION
            <param> [in] inCollisionMask  : Determine by GetCollisionMask() function in regard to 
                                            the group of this object
        */
            void                    SetCollisionGroupAndMask(uint32 inCollisionGroup,
                                                             uint32 inCollisionMask);
         // Set the capsule orientation to the vertical by default
            void                    SetCapsuleBaseOrientation(void);

        /// Functions
        /*
            Check if the entity has a transform component if not create one
            <return> the entity transform
        */
            Transform&	            CheckEntityTransform(void);

        // Preset of a box dynamic rigid body
            void                    CreateDynamicBoxRigidBody(void);
        // Preset of a sphere dynamic rigid body
            void                    CreateDynamicSphereRigidBody(void);
        // Preset of a capsule dynamic rigid body
            void                    CreateDynamicCapsuleRigidBody(void);

        /// Private members
        math::Vector3f          m_halfExtents       = math::Vector3f(0.5f, 0.5f, 0.5f);
        RigidBodyDynamicImpl*	m_rigidBodyImpl     = nullptr;
        Material*				m_materialImpl	    = nullptr;
        math::Vector2f          m_capsuleFormat     = math::Vector2f(0.5f, 1.f);
        f32                     m_radius            = 0.5f;
        bool                    m_isGravityDisabled = true;
        bool                    m_isTrigger         = false;
        bool                    m_isXLock           = false;
        bool                    m_isYLock           = false;
        bool                    m_isZLock           = false;

    }; // !Class RigidBodyDynamic

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<RigidBodyDynamic>()
    {
        return RIGIDBODY_DYNAMIC;
    }

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