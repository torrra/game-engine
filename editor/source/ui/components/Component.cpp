#include "ui/components/Component.h"
#include <engine/core/SceneGraph.h>
#include <engine/ui/UIComponent.h>

void editor::BaseComponent::RenderSection(engine::SceneGraph* graph, engine::EntityHandle const& handle)
{
    
    if (ui::CollapseSection(m_componentName.c_str(), m_isClosed))
    {
        ui::StartSection(m_componentName.c_str());
        SectionContent();
        ui::EndSection();
    }

    if (m_isClosed)
    {
        switch (m_componentType)
        {
        case editor::AUDIO:
            graph->DestroyComponent<engine::AudioPlayer>(handle);
            break;
        case editor::CAMERA:
            graph->DestroyComponent<engine::Camera>(handle);
            break;
        case editor::RENDERER:
            if (!graph->GetEntity(handle)->HasComponent<engine::TriangleMesh>())
            {
                graph->DestroyComponent<engine::Renderer>(handle);
            }
            break;
        case editor::RIGIDBODY_STATIC:
            graph->DestroyComponent<engine::RigidBodyStatic>(handle);
            break;
        case editor::RIGIDBODY_DYNAMIC:
            graph->DestroyComponent<engine::RigidBodyDynamic>(handle);
            break;
        case editor::TRANSFORM:
            if (!graph->GetEntity(handle)->HasComponent<engine::TriangleMesh>() &&
                !graph->GetEntity(handle)->HasComponent<engine::RigidBodyDynamic>() &&
                !graph->GetEntity(handle)->HasComponent<engine::RigidBodyStatic>())
            {
                graph->DestroyComponent<engine::Transform>(handle);
            }
            break;

        case editor::LIGHT_SOURCE:
            graph->DestroyComponent<engine::LightSource>(handle);
            break;

        case editor::TRIANGLE_MESH:
            graph->DestroyComponent<engine::TriangleMesh>(handle);
            break;

        case editor::INVALID_COMPONENT_TYPE:
        default:
            break;
        }

        printf("Removing component... %s\n", m_componentName.c_str());
        return;
    }
}

bool editor::BaseComponent::IsRemoved(void) const noexcept
{
    return m_isClosed;
}

void editor::BaseComponent::SetData(engine::SceneGraph* graph, engine::EntityHandle owner)
{
    m_graph = graph;
    m_owner = owner;
}

editor::EComponentType editor::BaseComponent::GetType(void) const noexcept
{
    return m_componentType;
}

void editor::BaseComponent::SectionContent(void)
{}

bool editor::BaseComponent::InputField(const char* uid, f32* value, f32 increment, f32 width, f32 xOffset)
{
    ui::SameLine(xOffset);
    ui::ItemWidth(width);
    return ui::InputBox(uid, value, increment);
}

void editor::BaseComponent::SetName(const char* name)
{
    m_componentName = name;
}

void editor::BaseComponent::SetType(EComponentType type)
{
    m_componentType = type;
}
