#include "ui/components/CameraComponent.h"

#include <engine/ui/UIComponent.h>

editor::CameraComponent::CameraComponent(void)
    : m_camera(nullptr)
{
    SetName("Camera");
}

editor::CameraComponent::~CameraComponent(void)
{
    m_camera = nullptr;
}

void editor::CameraComponent::SetCamera(engine::Camera* camera)
{
    m_camera = camera;
}

void editor::CameraComponent::SectionContent(void)
{
    m_fov = m_camera->GetFOV() * RAD2DEG;
    m_near = m_camera->GetNearPlane();
    m_far = m_camera->GetFarPlane();

    // FOV
    ui::Text("FOV: ");
    if (InputField("##FOV", &m_fov, 0.5f, 80.0f, 110.0f))
    {
        m_fov = math::Clamp(m_fov, 1.0f, 180.0f);
        m_camera->SetFOV(m_fov * DEG2RAD);
    }

    // Near plane
    ui::Text("Near plane: ");
    if (InputField("##Near", &m_near, 0.005f, 80.0f, 110.0f))
    {
        m_near = math::Clamp(m_near, 0.001f, m_far - 1.0f);
        m_camera->SetNearPlane(m_near);
    }
    ui::VerticalSpacing();
    
    // Far plane
    ui::Text("Far plane: ");
    if (InputField("##Far", &m_far, 0.5f, 80.0f, 110.0f))
    {
        m_far = math::Clamp(m_far, m_near + 1.0f, FLT_MAX);
        m_camera->SetFarPlane(m_far);
    }
    ui::VerticalSpacing();


}
