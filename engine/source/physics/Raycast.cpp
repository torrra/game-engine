#include "engine/physics/Raycast.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/rigidbody/RigidBodyDynamic.h"
#include "engine/physics/rigidbody/RigidBodyStatic.h"

#pragma endregion

#pragma region Engine

#include "engine/ConsoleLog.hpp"
#include "engine/Engine.h"

#pragma endregion

std::vector<engine::Raycast> engine::Raycast::m_existingRays;

engine::Raycast::Raycast(void)
{
    // Initialize the raycast implementation pointer
    m_raycastImpl                       = new RaycastImpl();
    // Initialize the raycast buffer
    m_raycastImpl->m_hit                = new physx::PxRaycastBuffer();
    // Initialize the raycast filter
    m_raycastImpl->m_queryFilterData    = physx::PxQueryFilterData(physx::PxQueryFlag::eNO_BLOCK);

    // Initialize the raycast values
    m_origin                            = math::Vector3f::Zero();
    m_direction                         = math::Vector3f(0.f, 0.f, 1.f);
    m_distance                          = 10.f;

    // Initialize the raycast lines for the pvd
    m_raycastImpl->m_successLine = new physx::PxDebugLine(ToPxVec3(m_origin), physx::PxVec3(0.f), 
                                                   physx::PxU32(physx::PxDebugColor::eARGB_GREEN));
    m_raycastImpl->m_failureLine = new physx::PxDebugLine(ToPxVec3(m_origin), physx::PxVec3(0.f), 
                                                   physx::PxU32(physx::PxDebugColor::eARGB_RED));
}

engine::Raycast::Raycast(Raycast&& inRaycast) noexcept
{
    m_direction = inRaycast.m_direction;
    m_distance = inRaycast.m_distance;
    m_origin = inRaycast.m_origin;
    m_index = inRaycast.m_index;
    m_raycastImpl = inRaycast.m_raycastImpl;

    inRaycast.m_raycastImpl = nullptr;
}

engine::Raycast::Raycast(const math::Vector3f& inOrigin, const math::Vector3f& inDirection,
                         f32 inDistance)
{
    // Initialize the raycast implementation pointer
    m_raycastImpl                       = new RaycastImpl();
    // Initialize the raycast buffer
    m_raycastImpl->m_hit                = new physx::PxRaycastBuffer();
    // Initialize the raycast filter
    m_raycastImpl->m_queryFilterData    = physx::PxQueryFilterData(physx::PxQueryFlag::eNO_BLOCK);

    // Initialize the raycast values
    m_origin                            = inOrigin;
    m_direction                         = inDirection;
    m_distance                          = inDistance;

    // Initialize the raycast lines for the pvd
    m_raycastImpl->m_successLine = new physx::PxDebugLine(ToPxVec3(m_origin), physx::PxVec3(0.f), 
                                                   physx::PxU32(physx::PxDebugColor::eARGB_GREEN));
    m_raycastImpl->m_failureLine = new physx::PxDebugLine(ToPxVec3(m_origin), physx::PxVec3(0.f), 
                                                   physx::PxU32(physx::PxDebugColor::eARGB_RED));

    SetFlags(engine::ERaycastFlags::ALL);
}

engine::Raycast::~Raycast(void)
{
    if (!m_raycastImpl)
        return;

    // Delete the raycast lines
    delete m_raycastImpl->m_successLine;
    m_raycastImpl->m_successLine = nullptr;
    delete m_raycastImpl->m_failureLine;
    m_raycastImpl->m_failureLine = nullptr;

    // Delete the raycast buffer
    delete m_raycastImpl->m_hit;
    m_raycastImpl->m_hit = nullptr;

    // Delete the raycast implementation
    delete m_raycastImpl;
    m_raycastImpl = nullptr;
}

const math::Vector3f& engine::Raycast::GetOrigin(void) const
{
    return m_origin;
}

const math::Vector3f& engine::Raycast::GetDirection(void) const
{
    return m_direction;
}

f32 engine::Raycast::GetDistance(void) const
{
    return m_distance;
}

void engine::Raycast::SetOrigin(const math::Vector3f& inOrigin)
{
    m_origin = inOrigin;
}

void engine::Raycast::SetDirection(const math::Vector3f& inDirection)
{
    m_direction = inDirection;
}

void engine::Raycast::SetDistance(f32 inDistance)
{
    m_distance = inDistance;
}

void engine::Raycast::SetRay(const math::Vector3f& inOrigin, const math::Vector3f& inDirection,
                             f32 inDistance)
{
    m_origin    = inOrigin;
    m_direction = inDirection;
    m_distance  = inDistance;
}

void engine::Raycast::SetFlags(ERaycastFlags inFlags)
{
    // Switch between the ERaycastFlags to set the raycast filter in regard to physx Query Flags
    switch (inFlags)
    {
    case engine::ERaycastFlags::DYNAMIC :
        // Set the raycast filter to detect dynamic objects with the raycast
        m_raycastImpl->m_queryFilterData = physx::PxQueryFilterData(physx::PxQueryFlag::eDYNAMIC);
        break;
    case engine::ERaycastFlags::STATIC :
        // Set the raycast filter to detect static objects with the raycast
        m_raycastImpl->m_queryFilterData = physx::PxQueryFilterData(physx::PxQueryFlag::eSTATIC);
        break;
    case engine::ERaycastFlags::ALL :
        // Set the raycast filter to detect all objects with the raycast
        m_raycastImpl->m_queryFilterData = physx::PxQueryFilterData(physx::PxQueryFlag::eDYNAMIC |
                                                                    physx::PxQueryFlag::eSTATIC);
        break;
    case engine::ERaycastFlags::NONE :
        // Set the raycast filter to detect no objects with the raycast
        m_raycastImpl->m_queryFilterData = physx::PxQueryFilterData(physx::PxQueryFlag::eNO_BLOCK);
        break;
    default:
        PrintLog(ErrorPreset(), "Invalid raycast flags.");
        break;
    }
}

bool engine::Raycast::HasHit(HitData* outData)
{

    if (math::AlmostEqual(m_direction.MagnitudeSquared(), 0.f))
        return false;

    /*
        Set the raycast hit flags    : To request hit fields to be filled in by scene queries 
                                             (such as hit position, normal, face index or UVs). 
                                     : Once query is completed, to indicate which fields are valid 
                                             (note that a query may produce more valid fields than 
                                             requested).
                                     : To specify additional options for the narrow phase and 
                                             mid-phase intersection routines.
    */
    physx::PxHitFlags hitFlags = physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL | 
                                 physx::PxHitFlag::eUV;

    /*
        Performs a raycast against objects in the scene, returns results in a PxRaycastBuffer object
		or via a custom user callback implementation inheriting from PxRaycastCallback.

		<param>[in] origin		:   Origin of the ray.
		<param>[in] unitDir		:   Normalized direction of the ray.
		<param>[in] distance	:	Length of the ray. Has to be in the [0, inf) range.
		<param>[out] hitCall	:	Raycast hit buffer or callback object used to report raycast 
                                        hits.
		<param>[in] hitFlags	:	Specifies which properties per hit should be computed and 
                                        returned via the hit callback.
		<param>[in] filterData  :	Filtering data passed to the filter shader. 
		<param>[in] filterCall  :	Custom filtering logic (optional). Only used if the 
                                        corresponding #PxQueryFlag flags are set. If NULL, all hits
                                        are assumed to be blocking.
		<param>[in] cache	    :   Cached hit shape (optional). Ray is tested against cached shape
                                        first. If no hit is found the ray gets queried against the
                                        scene.
							    	    Note: Filtering is not executed for a cached shape if 
                                        supplied; instead, if a hit is found, it is assumed to be a
                                        blocking hit.
								        Note: Using past touching hits as cache will produce 
                                        incorrect behavior since the cached hit will always be 
                                        treated as blocking.
		<param>[in] queryFlags	:   Optional flags controlling the query.

        <return> True if any touching or blocking hits were found or any hit was found in case 
                    PxQueryFlag::eANY_HIT was specified.
    */
    bool status = PhysicsEngine::Get().GetImpl().m_scene->raycast(ToPxVec3(m_origin),
                                                                  ToPxVec3(m_direction.Normalized()),
                                                                  m_distance, *m_raycastImpl->m_hit,
                                                                  hitFlags, 
                                                                  m_raycastImpl->m_queryFilterData);

    if (status && m_raycastImpl->m_hit->hasBlock)
    {
        if (outData)
        {
            const physx::PxRaycastHit& block = m_raycastImpl->m_hit->block;
            const RigidBodyData* actorData = reinterpret_cast<const RigidBodyData*>(&block.actor->userData);


            if (actorData->m_type == EShapeType::DYNAMIC)
            {
                RigidBodyDynamic& rigidbody =
                    *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + actorData->m_index);

                outData->m_hitEntity = rigidbody.GetOwner();
            }
            else
            {
                RigidBodyDynamic& rigidbody =
                    *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + actorData->m_index);

                outData->m_hitEntity = rigidbody.GetOwner();
            }

            outData->m_position = ToVector3f(block.position);
            outData->m_distance = block.distance;
        }

        return true;
    }

    return false;
}

void engine::Raycast::DrawRay(void)
{
    if (math::AlmostEqual(m_direction.MagnitudeSquared(), 0.f))
        return;

    // Convert the ray direction and origin to physx vectors
    physx::PxVec3 origin                = ToPxVec3(m_origin);
    physx::PxVec3 end                   = origin + ToPxVec3(m_direction.Normalized()) * m_distance;

    // Setup the raycast success and failure lines
    m_raycastImpl->m_successLine->pos0  = origin;
    m_raycastImpl->m_successLine->pos1  = end;
    m_raycastImpl->m_failureLine->pos0  = origin;
    m_raycastImpl->m_failureLine->pos1  = end;

    // Change the raycast line color in regard if the raycast hit something
    if (m_raycastImpl->m_hit->hasBlock)
    {
        // Set the raycast end to the hit position
        end = m_raycastImpl->m_hit->block.position;
        // Draw the line into the pvd
        PhysicsEngine::Get().GetImpl().m_scene->getScenePvdClient()->drawLines(
                m_raycastImpl->m_successLine, 1);
        // Add the line into the debug drawer to draw into opengl scene
        PhysicsEngine::Get().AddDebugLine(m_origin, ToVector3f(end), 
            physx::PxU32(physx::PxDebugColor::eARGB_GREEN));
    }
    else
    {
        // Draw the line into the pvd
        PhysicsEngine::Get().GetImpl().m_scene->getScenePvdClient()->drawLines(
            m_raycastImpl->m_failureLine, 1);
        // Add the line into the debug drawer to draw into opengl scene
        PhysicsEngine::Get().AddDebugLine(m_origin, ToVector3f(end), 
            physx::PxU32(physx::PxDebugColor::eARGB_RED));
    }
}

engine::Raycast& engine::Raycast::operator=(Raycast&& inOther) noexcept
{
    m_direction = inOther.m_direction;
    m_distance = inOther.m_distance;
    m_origin = inOther.m_origin;
    m_index = inOther.m_index;
    m_raycastImpl = inOther.m_raycastImpl;

    inOther.m_raycastImpl = nullptr;
    return *this;
}


int32 engine::Raycast::CreateRaycast(const math::Vector3f& inOrigin,
                                     const math::Vector3f& inDirection, f32 inDistance)
{
    int32 index = 0;

    for (; index < static_cast<int32>(m_existingRays.size()); ++index)
    {
        Raycast& ray = m_existingRays[index];

        if (ray.m_index == INVALID_INDEX)
        {
            ray.SetRay(inOrigin, inDirection, inDistance);
            ray.m_raycastImpl->m_queryFilterData = physx::PxQueryFilterData(physx::PxQueryFlag::eNO_BLOCK);
            *ray.m_raycastImpl->m_hit = physx::PxRaycastBuffer();
            return ray.m_index = index;
        }
    }

    Raycast& newRay = m_existingRays.emplace_back(inOrigin, inDirection, inDistance);
    return newRay.m_index = index;
}

void engine::Raycast::DestroyRay(int32 inIndex)
{
    if (inIndex < 0 || inIndex >= static_cast<int32>(m_existingRays.size()))
        return;

    m_existingRays[inIndex].m_index = INVALID_INDEX;
}

engine::Raycast* engine::Raycast::GetRay(int32 inIndex)
{
    if (inIndex < 0 || inIndex >= static_cast<int32>(m_existingRays.size()))
        return nullptr;

    Raycast& ray = m_existingRays[inIndex];
       
    if (ray.m_index == INVALID_INDEX)
        return nullptr;

    else
        return &ray;
}

void engine::Raycast::CleanupRays(void)
{
    m_existingRays.clear();
    m_existingRays.shrink_to_fit();
}

void engine::Raycast::DrawAllRays(void)
{
    for (Raycast& ray : m_existingRays)
        ray.DrawRay();
}

