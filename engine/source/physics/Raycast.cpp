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
    m_direction             = math::Vector3f::Zero();
    m_distance              = 0.f;

    m_pvdClient             = PhysicsEngine::Get().GetImpl().m_scene->getScenePvdClient();;
}

engine::Raycast::Raycast(const math::Vector3f& inOrigin, const math::Vector3f& inDirection,
                         const f32& inDistance)
{
    m_raycastImpl           = new RaycastImpl();
    m_raycastImpl->m_hit    = new physx::PxRaycastBuffer();

    m_origin                = inOrigin;
    m_direction             = inDirection;
    m_distance              = inDistance;

    m_pvdClient             = PhysicsEngine::Get().GetImpl().m_scene->getScenePvdClient();;
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

bool engine::Raycast::Hit(/*bool inStatus*/)
{
    bool status = PhysicsEngine::Get().GetImpl().m_scene->raycast(ToPxVec3(m_origin),
                                                                  ToPxVec3(m_direction.Normalized()),
                                                                  m_distance, *m_raycastImpl->m_hit,
                                                                  physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL, 
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
        static_cast<physx::PxPvdSceneClient*>(m_pvdClient)->drawLines(success, 1);
    }
    else
    {
        static_cast<physx::PxPvdSceneClient*>(m_pvdClient)->drawLines(failure, 1);
    }

    delete success;
    delete failure;
}
