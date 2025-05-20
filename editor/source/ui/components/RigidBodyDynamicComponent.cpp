#include "ui/components//RigidBodyDynamicComponent.h"

#pragma region Engine

#include <engine/ui/UIComponent.h>
#include <engine/ui/UIStyle.h>
#include <engine/Engine.h>

#pragma endregion

#pragma region Math

#include <math/Vector3.hpp>

#pragma endregion

std::vector<const char*> editor::RigidBodyDynamicComponent::m_shapeTypes = { "Box", "Sphere", "Capsule" };

editor::RigidBodyDynamicComponent::RigidBodyDynamicComponent(void)
{
    SetName("RigidbodyDynamic");
    SetType(RIGIDBODY_DYNAMIC);

    m_geometryName = "None##geometry";
}

editor::RigidBodyDynamicComponent::~RigidBodyDynamicComponent(void)
{
    m_geometryName.clear();
    m_shapeTypes.clear();
    SetData<engine::RigidBodyDynamic>(nullptr);
}

void editor::RigidBodyDynamicComponent::SectionContent(void)
{
    if (engine::RigidBodyDynamic* rigidBodyDynamic = GetData<engine::RigidBodyDynamic>())
    {
        engine::EGeometryType currentShape = static_cast<engine::EGeometryType>(rigidBodyDynamic->m_rigidBodyShape);
        
        int32 currentIndex = static_cast<int32>(currentShape);
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
        GravityUI(rigidBodyDynamic);
        TriggerUI(rigidBodyDynamic);
        LockAxisUI(rigidBodyDynamic);
    }
}

void editor::RigidBodyDynamicComponent::UpdateShapeGeometry(
                                        int32 inCurrentIndex, engine::EGeometryType inGeometryType,
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
            engine::RigidBodyDynamicFactory::CreateDynamic(engine::Engine::GetEngine()->GetGraph(),
                inRigidBodyDynamic->GetOwner(),
                engine::EGeometryType::BOX);
            break;
        case engine::EGeometryType::SPHERE:
            engine::RigidBodyDynamicFactory::CreateDynamic(engine::Engine::GetEngine()->GetGraph(),
                inRigidBodyDynamic->GetOwner(),
                engine::EGeometryType::SPHERE);
            break;
        case engine::EGeometryType::CAPSULE:
            engine::RigidBodyDynamicFactory::CreateDynamic(engine::Engine::GetEngine()->GetGraph(),
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

void editor::RigidBodyDynamicComponent::GravityUI(engine::RigidBodyDynamic* inRigidBodyDynamic)
{
    bool isGravityEnabled = inRigidBodyDynamic->IsGravityDisabled();
    const char* textGravity = "Disable gravity: ";
    math::Vector2f textGravitySize = ui::GetTextSize(textGravity); // gets text size (in pixels)
    ui::Text(textGravity);
    ui::SameLine(textGravitySize.GetX() + 5.0f);
    if (ui::Checkbox("##Gravity", &isGravityEnabled))
    {
        inRigidBodyDynamic->SetGravityDisabled(isGravityEnabled);
    }
}

void editor::RigidBodyDynamicComponent::TriggerUI(engine::RigidBodyDynamic* inRigidBodyDynamic)
{
    bool isTrigger = inRigidBodyDynamic->GetIsTrigger();
    const char* textTrigger = "Is trigger: ";
    math::Vector2f textTriggerSize = ui::GetTextSize(textTrigger);
    ui::Text(textTrigger);
    ui::SameLine(textTriggerSize.GetX() + 5.f);
    if (ui::Checkbox("##Trigger", &isTrigger))
    {
        inRigidBodyDynamic->SetTrigger(isTrigger);
    }
}

void editor::RigidBodyDynamicComponent::LockAxisUI(engine::RigidBodyDynamic* inRigidBodyDynamic)
{
    bool isXLock = inRigidBodyDynamic->GetIsXAxisLock();
    bool isYLock = inRigidBodyDynamic->GetIsYAxisLock();
    bool isZLock = inRigidBodyDynamic->GetIsZAxisLock();

    if (ui::TreeNode("Lock axis :"))
    {
        ui::StartSection("##Lock");
        const char* xAxis = "X Axis: ";
        math::Vector2f xAxisSize = ui::GetTextSize(xAxis);
        ui::Text(xAxis);
        ui::SameLine(xAxisSize.GetX() + 15.f);
        if (ui::Checkbox("##XLock", &isXLock))
        {
            inRigidBodyDynamic->SetXAxisLock(isXLock);
        }

        const char* yAxis = "Y Axis: ";
        math::Vector2f yAxisSize = ui::GetTextSize(yAxis);
        ui::Text(yAxis);
        ui::SameLine(yAxisSize.GetX() + 15.f);
        if (ui::Checkbox("##YLock", &isYLock))
        {
            inRigidBodyDynamic->SetYAxisLock(isYLock);
        }

        const char* zAxis = "Z Axis: ";
        math::Vector2f zAxisSize = ui::GetTextSize(zAxis);
        ui::Text(zAxis);
        ui::SameLine(zAxisSize.GetX() + 15.f);
        if (ui::Checkbox("##ZLock", &isZLock))
        {
            inRigidBodyDynamic->SetZAxisLock(isZLock);
        }
        ui::EndSection();

        ui::TreePop();
    }
}
