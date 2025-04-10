#include "engine/physics/Raycast.h"

engine::Raycast::Raycast(const math::Vector3f& inOrigin, const math::Vector3f& inDirection,
                         const f32& inDistance)
{
    m_origin    = inOrigin;
    m_direction = inDirection;
    m_distance  = inDistance;
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
