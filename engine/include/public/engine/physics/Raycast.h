#pragma once

#pragma region LibMath

#include "math/Vector3.hpp"

#pragma endregion

#pragma region Engine

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#pragma endregion

namespace engine
{
    struct RaycastImpl;

    // Enum to switch the raycast's flags
    enum class ERaycastFlags
    {
        DYNAMIC = 0,    // Collision between raycast and dynamic objects
        STATIC  = 1,    // Collision between raycast and static objects
        ALL     = 2,    // Collision between raycast and all objects
        NONE    = 3     // No collision
    }; // !Enum ERaycastFlags

    class Raycast
    {
    public :

        /// Constructor
        /*
            Default constructor
            m_origin = math::Vector3f(0.f, 0.f, 0.f)
            m_direction = math::Vector3f(0.f, 0.f, 1.f)
            m_distance = 10.f
        */
        ENGINE_API                          Raycast(void);
        // Delete copy constructor
                                            Raycast(const Raycast& inRaycast)   = delete;
        // Delete move constructor
                                            Raycast(Raycast&& inRaycast)        = delete;
        /*
            Constructor with given parameters
            <param> [in] inOrigin       : The origin of the ray     : math::Vector3f
            <param> [in] inDirection    : The direction of the ray  : math::Vector3f
            <param> [in] inDistance     : The distance of the ray   : f32 (optional : 10.f by 
                                                                                        default)
        */
        ENGINE_API                          Raycast(const math::Vector3f& inOrigin, 
                                                    const math::Vector3f& inDirection, 
                                                    f32 inDistance = 10.f);

        /// Destructor
        /*
            Delete the PxRaycastBuffer pointer
            Delete the RaycastImpl pointer
        */
        ENGINE_API                          ~Raycast(void);

        /// Getter
        /*
            Get the origin of the ray
            <return> [out] m_origin : The origin of the ray : math::Vector3f
        */
        ENGINE_API  const math::Vector3f&   GetOrigin(void) const;
        /*
            Get the direction of the ray
            <return> [out] m_direction : The direction of the ray : math::Vector3f
        */
        ENGINE_API  const math::Vector3f&   GetDirection(void) const;
        /*
            Get the distance of the ray
            <return> [out] m_distance : The distance of the ray : f32
        */
        ENGINE_API  f32                     GetDistance(void) const;

        /// Setter
        /*
            Set the origin of the ray
            <param> [in] inOrigin : The origin of the ray : math::Vector3f
        */
        ENGINE_API  void                    SetOrigin(const math::Vector3f& inOrigin);
        /*
            Set the direction of the ray
            <param> [in] inDirection : The direction of the ray : math::Vector3f
        */
        ENGINE_API  void                    SetDirection(const math::Vector3f& inDirection);
        /*
            Set the distance of the ray
            <param> [in] inDistance : The distance of the ray : f32
        */
        ENGINE_API  void                    SetDistance(f32 inDistance);
        /*
            Set the ray
            <param> [in] inOrigin       : The origin of the ray     : math::Vector3f
            <param> [in] inDirection    : The direction of the ray  : math::Vector3f
            <param> [in] inDistance     : The distance of the ray   : f32
        */
        ENGINE_API  void                    SetRay(const math::Vector3f& inOrigin, 
                                                   const math::Vector3f& inDirection, 
                                                   f32 inDistance = 10.f);
        /*
            Set the raycast's flag to use one or multiple or no collision detection
            <param> [in] inFlags : The flags to use : ERaycastFlags : DYNAMIC
                                                                    : STATIC
                                                                    : ALL
                                                                    : NONE
        */
        ENGINE_API  void                    SetFlags(ERaycastFlags inFlags);
        /// Functions
        /*
            Perform a check to see if the raycast hit an object
            <param> [in] inOutHit   : Used to report raycast hit : PxHitCallback<PxRaycastHit>
            <param> [out] status    : Return                     : true     : If there is a hit
                                                                            : false If there is no 
                                                                                hit
        */
        ENGINE_API  bool                    HasHit(void);
        // Draw the raycast into the pvd and the openGL scene
        ENGINE_API  void                    DrawRay(void);

    private :

        /// Private members
        // The origin of the ray
        math::Vector3f  m_origin        = math::Vector3f::Zero();
        // The direction of the ray
        math::Vector3f  m_direction     = math::Vector3f::Zero();
        // The raycast implementation
        RaycastImpl*    m_raycastImpl   = nullptr;
        // The distance of the ray
        f32             m_distance      = 0.f;

    }; // !Class Raycast
} // !Namespace engine