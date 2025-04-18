#include "ui/components/TransformComponent.h"
#include <engine/ui/UIComponent.h>
#include <math/Vector3.hpp>

editor::TransformComponent::TransformComponent(void)
    : m_transformData(nullptr)
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
    m_lastRot = m_rotation;
}

void editor::TransformComponent::SectionContent(void)
{
    math::Vector3f& position = m_transformData->SetPosition();
    math::Vector3f& size = m_transformData->SetScale();

    // Position
    ui::Text("Position: ");
    InputField("##X Position", &position[0], 0.05f);
    InputField("##Y Position", &position[1], 0.05f);
    InputField("##Z Position", &position[2], 0.05f);
    ui::VerticalSpacing();

    // Rotation
    ui::Text("Rotation: ");
    if (InputField("##X Rotation", &m_rotation[0], 0.5f))
    {
        m_transformData->AddRotation(m_rotation[0] - m_lastRot[0], 0.0f, 0.0f);
        m_lastRot[0] = m_rotation[0];
        m_rotation[0] = math::Wrap<f32>(m_rotation[0], 0.0f, 359.0f);
    }

    if (InputField("##Y Rotation", &m_rotation[1], 0.5f))
    {
        m_transformData->AddRotation(0.0f, m_rotation[1] - m_lastRot[1], 0.0f);
        m_lastRot[1] = m_rotation[1];
        m_rotation[1] = math::Wrap<f32>(m_rotation[1], 0.0f, 359.0f);
    }

    if (InputField("##Z Rotation", &m_rotation[2], 0.5f))
    {
        m_transformData->AddRotation(0.0f, 0.0f, m_rotation[2] - m_lastRot[2]);
        m_lastRot[2] = m_rotation[2];
        m_rotation[2] = math::Wrap<f32>(m_rotation[2], 0.0f, 359.0f);
    }
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

