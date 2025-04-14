#include "ui/components/TransformComponent.h"
#include <engine/ui/UIComponent.h>
#include <math/Vector3.hpp>

editor::TransformComponent::TransformComponent(void)
{
    SetName("Transform");
}

editor::TransformComponent::~TransformComponent(void)
{
    m_transformData = nullptr;
}

void editor::TransformComponent::SetTransform(engine::Transform* transform)
{
    m_transformData = transform;
    m_rotation = m_transformData->GetEulerRotation();
}

void editor::TransformComponent::SectionContent(void)
{
    math::Vector3f& position = m_transformData->SetPosition();
    math::Vector3f& size = m_transformData->SetScale();
    // 
    ui::Text("Position: ");
    InputField("##X Position", &position[0], 0.05f);
    InputField("##Y Position", &position[1], 0.05f);
    InputField("##Z Position", &position[2], 0.05f);

    bool isRotatedX, isRotatedY, isRotatedZ = false;
    ui::Text("Rotation: ");
    if (isRotatedX = InputField("##X Rotation", &m_rotation[0], 0.5f))
        m_rotation[0] = math::Wrap<f32>(m_rotation[0], 0.0f, 359.0f);
    if (isRotatedY = InputField("##Y Rotation", &m_rotation[1], 0.5f))
        m_rotation[1] = math::Wrap<f32>(m_rotation[1], 0.0f, 359.0f);
    if (isRotatedZ = InputField("##Z Rotation", &m_rotation[2], 0.5f))
        m_rotation[2] = math::Wrap<f32>(m_rotation[2], 0.0f, 359.0f);

    ui::Text("\tSize:");
    InputField("##X Size", &size[0], 0.05f);
    InputField("##Y Size", &size[1], 0.05f);
    InputField("##Z Size", &size[2], 0.05f);

    if (isRotatedX || isRotatedY || isRotatedZ)
        m_transformData->SetRotation(m_rotation.GetX(), m_rotation.GetY(), m_rotation.GetZ());

}

