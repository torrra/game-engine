#include "ui/components//RigidBodyDynamicComponent.h"

#pragma region Engine

#include <engine/ui/UIComponent.h>
#include <engine/physics/geometry/Geometry.hpp>
#include <engine/physics/rigidbody/RigidBodyDynamic.h>
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
        std::vector<const char*> shapeTypes = { "Box", "Sphere", "Capsule", "Plane" };
        int currentIndex = static_cast<int>(currentShape);

        currentShape = static_cast<engine::EGeometryType>(currentIndex);
        m_geometryName = shapeTypes[currentIndex];

        /// TODO : Reduce code

        if (ui::DropDown("##Shape Type", currentIndex, shapeTypes))
        {
            engine::EGeometryType newShape = static_cast<engine::EGeometryType>(currentIndex);

            if (newShape != currentShape)
            {
                currentShape = newShape;
                m_geometryName = shapeTypes[currentIndex];

                rigidBodyDynamic->RigidBodyDynamicCleanUp();

                switch (currentShape)
                {
                case engine::EGeometryType::BOX:
                    engine::RigidBodyDynamicFactory::CreateDynamic(m_engine->GetGraph(),
                                                                   rigidBodyDynamic->GetOwner(),
                                                                   engine::EGeometryType::BOX);
                    break;
                case engine::EGeometryType::SPHERE:
                    engine::RigidBodyDynamicFactory::CreateDynamic(m_engine->GetGraph(),
                                                                   rigidBodyDynamic->GetOwner(),
                                                                   engine::EGeometryType::SPHERE);
                    break;
                case engine::EGeometryType::CAPSULE:
                    engine::RigidBodyDynamicFactory::CreateDynamic(m_engine->GetGraph(),
                                                                   rigidBodyDynamic->GetOwner(),
                                                                   engine::EGeometryType::CAPSULE);
                    break;
                case engine::EGeometryType::PLANE:
                    engine::RigidBodyDynamicFactory::CreateDynamic(m_engine->GetGraph(),
                                                                   rigidBodyDynamic->GetOwner(),
                                                                   engine::EGeometryType::PLANE);
                    break;
                default:
                    break;
                }
            }
        }

        switch (currentShape)
        {
        case engine::EGeometryType::BOX:
        {
            math::Vector3f halfExtents = rigidBodyDynamic->GetBoxHalfExtents();
            ui::Text("Box format: ");
            InputField("##X", &halfExtents[0], 0.05f);
            InputField("##Y", &halfExtents[1], 0.05f);
            InputField("##Z", &halfExtents[2], 0.05f);
            rigidBodyDynamic->SetBoxHalfExtents(halfExtents);
            ui::VerticalSpacing();
            break;
        }
        case engine::EGeometryType::SPHERE:
        {
            f32 radius = rigidBodyDynamic->GetSphereRadius();
            ui::Text("Sphere format: ");
            InputField("##Radius", &radius, 0.05f);
            rigidBodyDynamic->SetSphereRadius(radius);
            ui::VerticalSpacing();
            break;
        }
        case engine::EGeometryType::CAPSULE:
        {
            f32 radius = rigidBodyDynamic->GetCapsuleFormat().GetX();
            f32 height = rigidBodyDynamic->GetCapsuleFormat().GetY();
            ui::Text("Capsule format: ");
            InputField("##Radius", &radius, 0.05f);
            InputField("##Height", &height, 0.05f);
            rigidBodyDynamic->SetCapsuleFormat(radius, height);
            ui::VerticalSpacing();
            break;
        }
        default:
            break;
        }
    }
}
