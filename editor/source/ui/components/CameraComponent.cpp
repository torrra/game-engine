#include "ui/components/CameraComponent.h"
#include <engine/ui/UIComponent.h>

editor::CameraComponent::CameraComponent(void)
    : m_rotationSynced(false)
{
    SetName("Camera");
    SetType(CAMERA);
}

editor::CameraComponent::~CameraComponent(void)
{
    m_rotationSynced = false;
}

void editor::CameraComponent::SectionContent(void)
{
    engine::Camera* camera = GetData<engine::Camera>();

    if (!m_rotationSynced)
    {
        SyncRotation(camera);
        m_rotationSynced = true;
    }

    Rotation(camera);
    FOV(camera);
    NearPlane(camera);
    FarPlane(camera);
}

void editor::CameraComponent::Rotation(engine::Camera* camera)
{
    ui::Text("Rotation: ");
    InputRotation(camera, "##X Rotation", 0);   // Pitch
    InputRotation(camera, "##Y Rotation", 1);   // Yaw
    InputRotation(camera, "##Z Rotation", 2);   // Roll
    ui::VerticalSpacing();
}

void editor::CameraComponent::FOV(engine::Camera* camera)
{
    f32 fov = camera->GetFOV() * RAD2DEG;

    // FOV
    ui::Text("FOV: ");
    if (InputField("##FOV", &fov, 0.5f, 80.0f, 110.0f))
    {
        fov = math::Clamp(fov, 1.0f, 180.0f);
        camera->SetFOV(fov * DEG2RAD);
    }
    ui::VerticalSpacing();
}

void editor::CameraComponent::NearPlane(engine::Camera* camera)
{
    f32 near = camera->GetNearPlane();
    f32 far = camera->GetFarPlane();


    // Near plane
    ui::Text("Near plane: ");
    if (InputField("##Near", &near, 0.005f, 80.0f, 110.0f))
    {
        near = math::Clamp(near, 0.001f, far - 1.0f);
        camera->SetNearPlane(near);
    }
    ui::VerticalSpacing();
}

void editor::CameraComponent::FarPlane(engine::Camera* camera)
{
    f32 near = camera->GetNearPlane();
    f32 far = camera->GetFarPlane();

    // Far plane
    ui::Text("Far plane: ");
    if (InputField("##Far", &far, 0.5f, 80.0f, 110.0f))
    {
        far = math::Clamp(far, near + 1.0f, FLT_MAX);
        camera->SetFarPlane(far);
    }
    ui::VerticalSpacing();
}

inline void editor::CameraComponent::InputRotation(engine::Camera* camera, const char* uid, uint8 index)
{
    if (InputField(uid, &m_rotation[index], 0.5f))
    {
        switch (index)
        {
        case 0:
            camera->Rotate(m_rotation[index] - m_lastRot[index], 0.0f, 0.0f, 1.0f);
            break;
        case 1:
            camera->Rotate(0.0f, m_rotation[index] - m_lastRot[index], 0.0f, 1.0f);
            break;
        case 2:
            camera->Rotate(0.0f, 0.0f, m_rotation[index] - m_lastRot[index], 1.0f);
            break;
        default:
            break;
        }

        m_lastRot[index] = m_rotation[index];
        m_rotation[index] = math::Wrap(m_rotation[index], 0.0f, 360.0f);
    }
}

inline void editor::CameraComponent::SyncRotation(engine::Camera* camera)
{
    // Rotation
    m_rotation = camera->GetRotation();

    m_rotation[0] = math::Wrap(m_rotation[0], 0.0f, 360.0f);
    m_rotation[1] = math::Wrap(m_rotation[1], 0.0f, 360.0f);
    m_rotation[2] = math::Wrap(m_rotation[2], 0.0f, 360.0f);

    m_lastRot = m_rotation;

}