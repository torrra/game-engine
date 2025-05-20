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

void engine::NavigationPoint::SerializeText(std::ostream& output, EntityHandle owner, uint64 index) const
{
    output << "[NavigationPoint]\n     ";

    if constexpr (UpdateAfterParent<NavigationPoint>::m_value)
    {
        text::Serialize(output, "index", index);
        output << "\n     ";
    }

    text::Serialize(output, "owner", owner);
    output << "\n     ";

    text::Serialize(output, "position", m_position);
    output << "\n     ";

    text::Serialize(output, "flags", m_flags);
    output << '\n';
}

const char* engine::NavigationPoint::DeserializeText(const char* text, const char* end)
{
    MOVE_TEXT_CURSOR(text, end);
    text = text::DeserializeInteger(text, m_owner);

    MOVE_TEXT_CURSOR(text, end);
    text = text::DeserializeVector(text, m_position);

    MOVE_TEXT_CURSOR(text, end);
    return text::DeserializeInteger(text, m_flags);
}

