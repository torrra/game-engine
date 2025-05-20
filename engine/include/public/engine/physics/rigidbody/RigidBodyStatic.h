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
#include "engine/physics/ICollisionListener.h"

#pragma endregion

#pragma region Math

#include "math/Vector3.hpp"
#include "math/Vector2.hpp"

#pragma endregion

namespace engine
{
	struct RigidBodyStaticImpl;

	class RigidBodyStatic : public ICollisionListener, public Component
	{
	public:

		/// Constructor		
        // Constructor with owner and scene parameters
		ENGINE_API				    
                                RigidBodyStatic(EntityHandle inOwner, 
                                                class SceneGraph* inScene);
        // Move constructor set to default
        ENGINE_API                  
                                RigidBodyStatic(RigidBodyStatic&&) noexcept = default;             

        /// Operator
        ENGINE_API
            RigidBodyStatic&    operator=(RigidBodyStatic&&) noexcept = default;


        ENGINE_API
            void                Invalidate(void) override;
        
        /// Getter
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
            Get if the rigidbody can be trigger
            <return> [out] bool :   true    = can be trigger
                                    false   = can not be trigger
        */
        ENGINE_API [[nodiscard]]
            bool                GetIsTrigger(void) const;

        /// Setter
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
            <param> [in] inRadius : the radius
            <param> [in] inHalfHeight : the half height
        */
        ENGINE_API  
            void                SetCapsuleFormat(f32 inRadius, f32 inHalfHeight);
        /*
            Set the debug draw visualization status
            <param> [in] inIsDebugVisualization : the visualization status : true = enabled
                                                                             false = disabled
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

		/// Functions
		ENGINE_API	
            void		        Register(void) override {}

        // Update the entity transform in reference to the dynamic rigid body
        ENGINE_API  
            void                UpdateEntity(void);
        // Update the dynamic rigid body transform in reference to the entity
        ENGINE_API  
            void                UpdateRigidBody(void);

		// Delete the dynamic rigid body resources
		ENGINE_API	
            void		        RigidBodyStaticCleanUp(void);

        /*
            Serialize rigidbody's member
            <param> [in] output : output stream
            <param> [in] owner  : Owner of the rigidbody component
            <param> [in] index  : Index of this component in an array of rigidbody component
        */
        ENGINE_API void			SerializeText(std::ostream& output, EntityHandle owner,
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
            void                OnCollisionStay([[maybe_unused]] EntityHandle inOther) override {}
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
            void                OnTriggerStay([[maybe_unused]] EntityHandle inOther) override {}
        /*
            Check the trigger collision at the end
            <param> [in] inOther : The other entity which enter in this trigger
        */
        ENGINE_API  
            void                OnTriggerExit(EntityHandle inOther) override;

        /// Public members
        // RigidBody shape { Box, Sphere, Capsule }
        uint64                      m_rigidBodyShape    = 0;
        // Struct to store index and rigidBody type
        RigidBodyData               m_data;
        // Rigidbody type { Dynamic, Static }
        uint32                      m_rigidBodyType     = EShapeType::STATIC;
        // Collision group { None, Default, Projectile, Environment, Trigger }
        collision::ECollisionGroup  m_collisionGroup    =
                                                collision::ECollisionGroup::ENVIRONMENT_COLLISION;

	private :

        /// Friend class
        friend class RigidBodyStaticFactory;

        /// Constructor
        // Default constructor deleted
                                RigidBodyStatic(void) = delete;
        // Copy constructor deleted
                                RigidBodyStatic(const RigidBodyStatic& inOther) = delete;

        /// Operator
        // Copy assignement deleted
            RigidBodyStatic&    operator=(const RigidBodyStatic& inOther) = delete;

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
            void                SetCollisionGroupAndMask(uint32 inCollisionGroup,
                                                         uint32 inCollisionMask);
        // Set the capsule orientation to the vertical by default
            void                SetCapsuleBaseOrientation(void);

		/// Functions
        /*
            Check if the entity has a transform component if not create one
            <return> the entity transform
        */
            Transform&          CheckEntityTransform(void);

        // Preset of a box dynamic rigid body
            void                CreateStaticBoxRigidBody(void);
        // Preset of a sphere dynamic rigid body
            void                CreateStaticSphereRigidBody(void);
        // Preset of a capsule dynamic rigid body
            void                CreateStaticCapsuleRigidBody(void);
        // Preset of a plane static rigid body
            void                CreateStaticPlaneRigidBody(void);
        
        /// Private members
        math::Vector3f          m_halfExtents           = math::Vector3f(0.5f, 0.5f, 0.5f);
		RigidBodyStaticImpl*    m_rigidBodyStaticImpl   = nullptr;
        Material*               m_materialImpl          = nullptr;
        math::Vector2f          m_capsuleFormat         = math::Vector2f(0.5f, 1.f);
        f32                     m_radius                = 0.5f;
        bool                    m_isTrigger             = false;

	}; // !Class RigidBodyStatic

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<RigidBodyStatic>()
    {
        return RIGIDBODY_STATIC;
    }

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
        ENGINE_API static RigidBodyStatic* CreateStatic(class SceneGraph* inScene, 
                                                        EntityHandle inOwner,
                                                        const EGeometryType& inGeometry);
    }; // !Class RigidBodyStaticFactory
} // !Namespace engine