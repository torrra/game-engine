#include "ui/components/RigidBodyStaticComponent.h"

#pragma region Engine

#include <engine/ui/UIComponent.h>
#include <engine/ui/UIStyle.h>
#include <engine/Engine.h>

#pragma endregion

editor::RigidBodyStaticComponent::RigidBodyStaticComponent(void)
{
    SetName("RigidbodyStatic");
    SetType(RIGIDBODY_STATIC);

    m_geometryName = "None##geometry";
}

editor::RigidBodyStaticComponent::~RigidBodyStaticComponent(void)
{
}

void editor::RigidBodyStaticComponent::SectionContent(void)
{
    if (engine::RigidBodyStatic* rigidBodyStatic = GetData<engine::RigidBodyStatic>())
    {
        engine::EGeometryType currentShape = static_cast<engine::EGeometryType>(rigidBodyStatic->m_rigidBodyShape);
        
        int32 currentIndex = static_cast<int32>(currentShape);
        m_geometryName = m_shapeTypes[currentIndex];

        const char* text = "Collider type: "; // text for the label
        math::Vector2f textSize = ui::GetTextSize(text); // gets text size (in pixels)
        ui::Text(text); // render text
        ui::SameLine(textSize.GetX() + 5.0f);

        if (ui::DropDown("##Shape Type", currentIndex, m_shapeTypes))
        {
            UpdateShapeGeometry(currentIndex, currentShape, rigidBodyStatic);
        }

        DisplayUI(currentShape, rigidBodyStatic);

        if (currentShape != engine::EGeometryType::PLANE)
        {
            bool isTrigger = rigidBodyStatic->GetIsTrigger();
            ui::Text("Is trigger: ");
            ui::SameLine(125.f);
            if (ui::Checkbox("##Trigger", &isTrigger))
            {
                rigidBodyStatic->SetTrigger(isTrigger);
            }
        }
    }
}

void editor::RigidBodyStaticComponent::UpdateShapeGeometry(
                                                   int32 inCurrentIndex, 
                                                   engine::EGeometryType inGeometryType,
                                                   engine::RigidBodyStatic* inRigidBodyStatic)
{
    engine::EGeometryType newShape = static_cast<engine::EGeometryType>(inCurrentIndex);

    if (newShape != inGeometryType)
    {
        inGeometryType = newShape;
        m_geometryName = m_shapeTypes[inCurrentIndex];

        inRigidBodyStatic->RigidBodyStaticCleanUp();

        switch (inGeometryType)
        {
        case engine::EGeometryType::BOX:
            engine::RigidBodyStaticFactory::CreateStatic(engine::Engine::GetEngine()->GetGraph(),
                inRigidBodyStatic->GetOwner(),
                engine::EGeometryType::BOX);
            break;
        case engine::EGeometryType::SPHERE:
            engine::RigidBodyStaticFactory::CreateStatic(engine::Engine::GetEngine()->GetGraph(),
                inRigidBodyStatic->GetOwner(),
                engine::EGeometryType::SPHERE);
            break;
        case engine::EGeometryType::CAPSULE:
            engine::RigidBodyStaticFactory::CreateStatic(engine::Engine::GetEngine()->GetGraph(),
                inRigidBodyStatic->GetOwner(),
                engine::EGeometryType::CAPSULE);
            break;
        case engine::EGeometryType::PLANE:
            engine::RigidBodyStaticFactory::CreateStatic(engine::Engine::GetEngine()->GetGraph(),
                inRigidBodyStatic->GetOwner(),
                engine::EGeometryType::PLANE);
            break;
        default:
            break;
        }
    }
}

void editor::RigidBodyStaticComponent::DisplayUI(engine::EGeometryType inGeometryType, 
                                                 engine::RigidBodyStatic* inRigidBodyStatic)
{
    switch (inGeometryType)
    {
    case engine::EGeometryType::BOX:
    {
        math::Vector3f halfExtents = inRigidBodyStatic->GetBoxHalfExtents();
        ui::Text("Box format: ");
        InputField("##X", &halfExtents[0], 0.05f);
        InputField("##Y", &halfExtents[1], 0.05f);
        InputField("##Z", &halfExtents[2], 0.05f);
        inRigidBodyStatic->SetBoxHalfExtents(halfExtents);
        ui::VerticalSpacing();
        break;
    }
    case engine::EGeometryType::SPHERE:
    {
        f32 radius = inRigidBodyStatic->GetSphereRadius();
        ui::Text("Sphere format: ");
        InputField("##Radius", &radius, 0.05f);
        inRigidBodyStatic->SetSphereRadius(radius);
        ui::VerticalSpacing();
        break;
    }
    case engine::EGeometryType::CAPSULE:
    {
        f32 radius = inRigidBodyStatic->GetCapsuleFormat().GetX();
        f32 height = inRigidBodyStatic->GetCapsuleFormat().GetY();
        ui::Text("Capsule format: ");
        InputField("##Radius", &radius, 0.05f);
        InputField("##Height", &height, 0.05f);
        inRigidBodyStatic->SetCapsuleFormat(radius, height);
        ui::VerticalSpacing();
        break;
    }
    default:
        break;
    }
}
