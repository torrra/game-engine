#include "engine/core/components/NavigationPoint.h"

#pragma region Serialization

#include "serialization/TextSerializer.h"

#pragma endregion

#pragma region Resource

#include "engine/resource/shader/Shader.h"
#include "engine/resource/ResourceManager.h"
#include "engine/resource/model/Model.h"

#pragma endregion

#pragma region System

#include "engine/core/systems/ScriptSystem.h"

#pragma endregion

engine::NavigationPoint::NavigationPoint(EntityHandle inOwner, SceneGraph* inScene)
{
    inOwner = m_owner;
    inScene = m_currentScene;

    ResourceManager::Load<Model>(".\\assets\\sphereLP.obj", true);

    ResourceManager::LoadShader("NavPointProgram", ".\\shaders\\NavPointShader.vert",
                                ".\\shaders\\NavPointShader.frag", true, true);
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

void engine::NavigationPoint::Register(void)
{
    engine::ScriptSystem::RegisterNewComponent("_NewNavigationPointComponent", m_owner);
}

void engine::NavigationPoint::Unregister(void)
{
    engine::ScriptSystem::UnregisterComponent("_RemoveNavigationPointComponent", m_owner);
}

void engine::NavigationPoint::RenderNavPoint(const math::Matrix4f& inProjView)
{
    ResourceManager::GetResource<ShaderProgram>("NavPointProgram")->Use();

    math::Matrix4f scale = math::Matrix4f::ScaleMatrix(math::Vector3f(0.2f));
    math::Matrix4f translate = math::Matrix4f::PositionMatrix(GetPosition());
    math::Matrix4f model = translate * scale;
    math::Matrix4f mvp = inProjView * model;

    ResourceManager::GetResource<ShaderProgram>("NavPointProgram")->Set("mvp", &mvp);
    ResourceManager::GetResource<ShaderProgram>("NavPointProgram")->Set("aColor", math::Vector4f(1.f, 0.f, 0.f, 1.f));

    ResourceManager::GetResource<Model>(".\\assets\\sphereLP.obj")->Draw();
}