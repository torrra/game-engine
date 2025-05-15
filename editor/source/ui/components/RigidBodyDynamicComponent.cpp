#include "ui/components//RigidBodyDynamicComponent.h"

#pragma region Engine

#include <engine/ui/UIComponent.h>
#include <engine/ui/UIStyle.h>
#include <engine/Engine.h>

#pragma endregion

editor::RigidBodyDynamicComponent::RigidBodyDynamicComponent(void)
{
    SetName("RigidbodyDynamic");
    SetType(RIGIDBODY_DYNAMIC);

    m_geometryName = "None##geometry";
    m_engine = engine::Engine::GetEngine();
}

editor::RigidBodyDynamicComponent::~RigidBodyDynamicComponent(void)
{
    SetData<engine::RigidBodyDynamic>(nullptr);
}

void editor::RigidBodyDynamicComponent::SectionContent(void)
{
    if (engine::RigidBodyDynamic* rigidBodyDynamic = GetData<engine::RigidBodyDynamic>())
    {
        engine::EGeometryType currentShape = static_cast<engine::EGeometryType>(rigidBodyDynamic->m_shape);
        
        int currentIndex = static_cast<int>(currentShape);
        m_geometryName = m_shapeTypes[currentIndex];

        const char* text = "Collider type: "; // text for the label
        math::Vector2f textSize = ui::GetTextSize(text); // gets text size (in pixels)
        ui::Text(text); // render text
        ui::SameLine(textSize.GetX() + 5.0f); // make next component on same line starting at the position of the width + 5 ( padding)


        if (ui::DropDown("##Shape Type", currentIndex, m_shapeTypes))
        {
            UpdateShapeGeometry(currentIndex, currentShape, rigidBodyDynamic);
        }

        DisplayUI(currentShape, rigidBodyDynamic);

        bool isGravityEnabled = rigidBodyDynamic->IsGravityDisabled();
        ui::Text("Disable gravity: ");
        ui::SameLine(125.f);
        if (ui::Checkbox("##Gravity", &isGravityEnabled))
        {
            rigidBodyDynamic->SetGravityDisabled(isGravityEnabled);
        }
    }
}

void editor::RigidBodyDynamicComponent::UpdateShapeGeometry(
                                        int inCurrentIndex, engine::EGeometryType inGeometryType,
                                        engine::RigidBodyDynamic* inRigidBodyDynamic)
{
    engine::EGeometryType newShape = static_cast<engine::EGeometryType>(inCurrentIndex);

    if (newShape != inGeometryType)
    {
        inGeometryType = newShape;
        m_geometryName = m_shapeTypes[inCurrentIndex];

        inRigidBodyDynamic->RigidBodyDynamicCleanUp();

        switch (inGeometryType)
        {
        case engine::EGeometryType::BOX:
            engine::RigidBodyDynamicFactory::CreateDynamic(m_engine->GetGraph(),
                inRigidBodyDynamic->GetOwner(),
                engine::EGeometryType::BOX);
            break;
        case engine::EGeometryType::SPHERE:
            engine::RigidBodyDynamicFactory::CreateDynamic(m_engine->GetGraph(),
                inRigidBodyDynamic->GetOwner(),
                engine::EGeometryType::SPHERE);
            break;
        case engine::EGeometryType::CAPSULE:
            engine::RigidBodyDynamicFactory::CreateDynamic(m_engine->GetGraph(),
                inRigidBodyDynamic->GetOwner(),
                engine::EGeometryType::CAPSULE);
            break;
        default:
            break;
        }
    }
}

void editor::RigidBodyDynamicComponent::DisplayUI(engine::EGeometryType inGeometryType,
                                                  engine::RigidBodyDynamic* inRigidBodyDynamic)
{
    switch (inGeometryType)
    {
    case engine::EGeometryType::BOX:
    {
        math::Vector3f halfExtents = inRigidBodyDynamic->GetBoxHalfExtents();
        ui::Text("Box format: ");
        InputField("##X", &halfExtents[0], 0.05f);
        InputField("##Y", &halfExtents[1], 0.05f);
        InputField("##Z", &halfExtents[2], 0.05f);
        inRigidBodyDynamic->SetBoxHalfExtents(halfExtents);
        ui::VerticalSpacing();
        break;
    }
    case engine::EGeometryType::SPHERE:
    {
        f32 radius = inRigidBodyDynamic->GetSphereRadius();
        ui::Text("Sphere format: ");
        InputField("##Radius", &radius, 0.05f);
        inRigidBodyDynamic->SetSphereRadius(radius);
        ui::VerticalSpacing();
        break;
    }
    case engine::EGeometryType::CAPSULE:
    {
        f32 radius = inRigidBodyDynamic->GetCapsuleFormat().GetX();
        f32 height = inRigidBodyDynamic->GetCapsuleFormat().GetY();
        ui::Text("Capsule format: ");
        InputField("##Radius", &radius, 0.05f);
        InputField("##Height", &height, 0.05f);
        inRigidBodyDynamic->SetCapsuleFormat(radius, height);
        ui::VerticalSpacing();
        break;
    }
    default:
        break;
    }
}
