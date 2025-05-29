#include "ui/components/TransformComponent.h"
#include <engine/ui/UIComponent.h>
#include <math/Vector3.hpp>

editor::TransformComponent::TransformComponent(void)
{
    SetName("Transform");
    SetType(TRANSFORM);
}

void editor::TransformComponent::SectionContent(void)
{
    engine::Transform* transform = GetData<engine::Transform>();
    math::Vector3f& position = transform->SetPosition();
    math::Vector3f& size = transform->SetScale();
    
    SyncRotation(transform);

    // Position
    ui::Text("Position: ");
    InputField("##X Position", &position[0], 0.05f);
    InputField("##Y Position", &position[1], 0.05f);
    InputField("##Z Position", &position[2], 0.05f);
    ui::VerticalSpacing();

    // Rotation
    ui::Text("Rotation: ");
    InputRotation(transform, "##X Rotation", 0);
    InputRotation(transform, "##Y Rotation", 1);
    InputRotation(transform, "##Z Rotation", 2);
    ui::VerticalSpacing();

    // Size
    ui::Text("\tSize: ");
    if (InputField("##X Size", &size[0], 0.05f))
        size[0] = math::Clamp(size[0], 0.001f, FLT_MAX);
    if (InputField("##Y Size", &size[1], 0.05f))
        size[1] = math::Clamp(size[1], 0.001f, FLT_MAX);
    if (InputField("##Z Size", &size[2], 0.05f))
        size[2] = math::Clamp(size[2], 0.001f, FLT_MAX);
    ui::VerticalSpacing();

}

void editor::TransformComponent::SyncRotation(engine::Transform* transform)
{
    // Rotation
    m_rotation = transform->GetEulerRotation();

    m_rotation[0] = math::Wrap(m_rotation[0], 0.0f, 360.0f);
    m_rotation[1] = math::Wrap(m_rotation[1], 0.0f, 360.0f);
    m_rotation[2] = math::Wrap(m_rotation[2], 0.0f, 360.0f);

    m_lastRot = m_rotation;
}

void editor::TransformComponent::InputRotation(engine::Transform* transform, const char* uid, uint8 index)
{
    if (InputField(uid, &m_rotation[index], 0.5f))
    {
        switch (index)
        {
        case 0:
            transform->AddRotation(m_rotation[index] - m_lastRot[index], 0.0f, 0.0f);
            break;
        case 1:
            transform->AddRotation(0.0f, m_rotation[index] - m_lastRot[index], 0.0f);
            break;
        case 2:
            transform->AddRotation(0.0f, 0.0f, m_rotation[index] - m_lastRot[index]);
            break;
        default:
            break;
        }
        
        m_lastRot[index] = m_rotation[index];
        m_rotation[index] = math::Wrap(m_rotation[index], 0.0f, 360.0f);
    }
}
