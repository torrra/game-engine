#pragma once

#pragma region LibMath

#include "math/Vector3.hpp"

#pragma endregion

#pragma region Engine

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#pragma endregion


#include <vector>

#pragma endregion

namespace engine
{
    struct RaycastImpl;

    class Raycast
    {
    public :

        /// Constructor
        // Default constructor
        ENGINE_API                  Raycast(void);
        // Delete copy constructor
                                    Raycast(const Raycast& inRaycast) = delete;
        // Delete move constructor
                                    Raycast(Raycast&& inRaycast) = delete;
        /*
            Constructor with given parameters
            <param> [in] inOrigin       : The origin of the ray     : math::Vector3f
            <param> [in] inDirection    : The direction of the ray  : math::Vector3f
            <param> [in] inDistance     : The distance of the ray   : f32
        */
        ENGINE_API                  Raycast(const math::Vector3f& inOrigin, 
                                            const math::Vector3f& inDirection, 
                                            const f32& inDistance = 10.f);

        /// Destructor
        ENGINE_API                  ~Raycast(void);

        /// Getter
        /*
            Get the origin of the ray
            <return> [out] m_origin : The origin of the ray : math::Vector3f
        */
        ENGINE_API  math::Vector3f  GetOrigin(void) const;
        /*
            Get the direction of the ray
            <return> [out] m_direction : The direction of the ray : math::Vector3f
        */
        ENGINE_API  math::Vector3f  GetDirection(void) const;
        /*
            Get the distance of the ray
            <return> [out] m_distance : The distance of the ray : f32
        */
        ENGINE_API  f32             GetDistance(void) const;

        /// Setter
        /*
            Set the origin of the ray
            <param> [in] inOrigin : The origin of the ray : math::Vector3f
        */
        ENGINE_API  void            SetOrigin(const math::Vector3f& inOrigin);
        /*
            Set the direction of the ray
            <param> [in] inDirection : The direction of the ray : math::Vector3f
        */
        ENGINE_API  void            SetDirection(const math::Vector3f& inDirection);
        /*
            Set the distance of the ray
            <param> [in] inDistance : The distance of the ray : f32
        */
        ENGINE_API  void            SetDistance(const f32& inDistance);
        /*
            Set the ray
            <param> [in] inOrigin       : The origin of the ray     : math::Vector3f
            <param> [in] inDirection    : The direction of the ray  : math::Vector3f
            <param> [in] inDistance     : The distance of the ray   : f32
        */
        ENGINE_API  void            SetRay(const math::Vector3f& inOrigin, 
                                           const math::Vector3f& inDirection, 
                                           const f32& inDistance = 10.f);

        /// Functions
        /*
            Perform a check to see if the raycast hit an object
            <param> [in] inOutHit   : Used to report raycast hit : PxHitCallback<PxRaycastHit>
            <param> [out] status    : Return                     : true     : If there is a hit
                                                                            : false If there is no hit
        */
        ENGINE_API  bool            HasHit(void);
        ENGINE_API  void            DrawRayInPvd(void);
        /// Public members
        std::vector<void*> m_raycastLines;

    private :

        /// Private members
        RaycastImpl*    m_raycastImpl   = nullptr;
        // The origin of the ray
        math::Vector3f  m_origin        = math::Vector3f::Zero();
        // The direction of the ray
        math::Vector3f  m_direction     = math::Vector3f::Zero();
        // The distance of the ray
        f32             m_distance      = 0.f;

    }; // !Class Raycast
} // !Namespace engine