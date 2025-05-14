#include "ui/components/RigidBodyStaticComponent.h"

#pragma region Engine

#include <engine/ui/UIComponent.h>
#include <engine/physics/geometry/Geometry.hpp>
#include <engine/physics/rigidbody/RigidBodyStatic.h>
#include <engine/Engine.h>

#pragma endregion

editor::RigidBodyStaticComponent::RigidBodyStaticComponent(void)
{
    SetName("RigidbodyStatic");
    SetType(RIGIDBODY_STATIC);

    m_geometryName = "None##geometry";
    m_engine = engine::Engine::GetEngine();
}

editor::RigidBodyStaticComponent::~RigidBodyStaticComponent(void)
{
    SetData<engine::RigidBodyStatic>(nullptr);
}

void editor::RigidBodyStaticComponent::SectionContent(void)
{
    if (engine::RigidBodyStatic* rigidBodyStatic = GetData<engine::RigidBodyStatic>())
    {
        engine::EGeometryType currentShape = static_cast<engine::EGeometryType>(rigidBodyStatic->m_shape);
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

                rigidBodyStatic->RigidBodyStaticCleanUp();

                switch (currentShape)
                {
                case engine::EGeometryType::BOX:
                    engine::RigidBodyStaticFactory::CreateStatic(m_engine->GetGraph(),
                                                                 rigidBodyStatic->GetOwner(), 
                                                                 engine::EGeometryType::BOX);                    
                    break;
                case engine::EGeometryType::SPHERE:
                    engine::RigidBodyStaticFactory::CreateStatic(m_engine->GetGraph(),
                                                                 rigidBodyStatic->GetOwner(),
                                                                 engine::EGeometryType::SPHERE);
                    break;
                case engine::EGeometryType::CAPSULE:
                    engine::RigidBodyStaticFactory::CreateStatic(m_engine->GetGraph(),
                                                                 rigidBodyStatic->GetOwner(),
                                                                 engine::EGeometryType::CAPSULE);
                    break;
                case engine::EGeometryType::PLANE:
                    engine::RigidBodyStaticFactory::CreateStatic(m_engine->GetGraph(),
                                                                 rigidBodyStatic->GetOwner(),
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
            math::Vector3f halfExtents = rigidBodyStatic->GetBoxHalfExtents();
            ui::Text("Box format: ");
            InputField("##X", &halfExtents[0], 0.05f);
            InputField("##Y", &halfExtents[1], 0.05f);
            InputField("##Z", &halfExtents[2], 0.05f);
            rigidBodyStatic->SetBoxHalfExtents(halfExtents);
            ui::VerticalSpacing();
            break;
        }
        case engine::EGeometryType::SPHERE:
        {
            f32 radius = rigidBodyStatic->GetSphereRadius();
            ui::Text("Sphere format: ");
            InputField("##Radius", &radius, 0.05f);
            rigidBodyStatic->SetSphereRadius(radius);
            ui::VerticalSpacing();
            break;
        }
        case engine::EGeometryType::CAPSULE:
        {
            f32 radius = rigidBodyStatic->GetCapsuleFormat().GetX();
            f32 height = rigidBodyStatic->GetCapsuleFormat().GetY();
            ui::Text("Capsule format: ");
            InputField("##Radius", &radius, 0.05f);
            InputField("##Height", &height, 0.05f);
            rigidBodyStatic->SetCapsuleFormat(radius, height);
            ui::VerticalSpacing();
            break;
        }
        default:
            break;
        }
    }
}
