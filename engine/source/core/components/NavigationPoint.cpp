#include "engine/core/components/NavigationPoint.h"

#pragma region Serialization

#include "serialization/TextSerializer.h"

#pragma endregion

#pragma region Internal

#include "InternalOpenGLError.hpp"

#pragma endregion

#pragma region Resource

#include "engine/resource/shader/Shader.h"
#include "engine/resource/ResourceManager.h"

#pragma endregion

engine::NavigationPoint::NavigationPoint(EntityHandle inOwner, SceneGraph* inScene)
{
    inOwner = m_owner;
    inScene = m_currentScene;
}

math::Vector3f engine::NavigationPoint::GetPosition(void) const
{
    return m_position;
}

void engine::NavigationPoint::SetPosition(const math::Vector3f& inPosition)
{
    m_position = inPosition;
}
