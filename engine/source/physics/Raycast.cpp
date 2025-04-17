#include "engine/physics/Raycast.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"

#pragma endregion

#pragma region Engine

#include "engine/ConsoleLog.hpp"

#pragma endregion

engine::Raycast::Raycast(void)
{
    m_raycastImpl           = new RaycastImpl();
    m_raycastImpl->m_hit    = new physx::PxRaycastBuffer();

    m_origin                = math::Vector3f::Zero();
    m_direction             = math::Vector3f(0.f, 0.f, 1.f);
    m_distance              = 10.f;
}

engine::Raycast::Raycast(const math::Vector3f& inOrigin, const math::Vector3f& inDirection,
                         const f32& inDistance)
{
    m_raycastImpl           = new RaycastImpl();
    m_raycastImpl->m_hit    = new physx::PxRaycastBuffer();

    m_origin                = inOrigin;
    m_direction             = inDirection;
    m_distance              = inDistance;
}

engine::Raycast::~Raycast(void)
{
    delete m_raycastImpl->m_hit;
    m_raycastImpl->m_hit = nullptr;

    delete m_raycastImpl;
    m_raycastImpl = nullptr;
}

math::Vector3f engine::Raycast::GetOrigin(void) const
{
    return m_origin;
}

math::Vector3f engine::Raycast::GetDirection(void) const
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

void engine::Raycast::SetDistance(const f32& inDistance)
{
    m_distance = inDistance;
}

void engine::Raycast::SetRay(const math::Vector3f& inOrigin, const math::Vector3f& inDirection,
                             const f32& inDistance)
{
    m_origin    = inOrigin;
    m_direction = inDirection;
    m_distance  = inDistance;
}

bool engine::Raycast::HasHit(void)
{
    physx::PxHitFlags hitFlags = physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL | 
                                 physx::PxHitFlag::eUV;
    bool status = PhysicsEngine::Get().GetImpl().m_scene->raycast(ToPxVec3(m_origin),
                                                                  ToPxVec3(m_direction.Normalized()),
                                                                  m_distance, *m_raycastImpl->m_hit,
                                                                  hitFlags, 
                                                                  physx::PxQueryFilterData(physx::PxQueryFlag::eDYNAMIC));

    if (status && m_raycastImpl->m_hit->hasBlock)
    {
        PrintLog(SuccessPreset(), "Raycast hit something !");
        return true;
    }

    PrintLog(ErrorPreset(), "Raycast hit nothing...");
    return false;
}

void engine::Raycast::DrawRay(void)
{
    physx::PxVec3 end = ToPxVec3(m_origin) + ToPxVec3(m_direction.Normalized()) * m_distance;

    physx::PxDebugLine* success = new physx::PxDebugLine(ToPxVec3(m_origin), end, 0xff008800);
    physx::PxDebugLine* failure = new physx::PxDebugLine(ToPxVec3(m_origin), end, 0xff880000);

    if (m_raycastImpl->m_hit->hasBlock)
    {
        end = m_raycastImpl->m_hit->block.position;
        PhysicsEngine::Get().GetImpl().m_scene->getScenePvdClient()->drawLines(success, 1);
        PhysicsEngine::Get().AddDebugLine(m_origin, ToVector3f(end), 0xff008800);
    }
    else
    {
        PhysicsEngine::Get().GetImpl().m_scene->getScenePvdClient()->drawLines(failure, 1);
        PhysicsEngine::Get().AddDebugLine(m_origin, ToVector3f(end), 0xff880000);
    }

    delete success;
    delete failure;
}
