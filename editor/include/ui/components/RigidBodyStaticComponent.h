#pragma once

#pragma region Editor

#include "Component.h"

#pragma endregion

#pragma region Engine

#include <engine/CoreTypes.h>
#include <engine/physics/geometry/Geometry.hpp>
#include <engine/physics/rigidbody/RigidBodyStatic.h>
#include <engine/CoreTypes.h>

#pragma endregion

#pragma region Standard

#include <vector>
#include <string>

#pragma endregion

namespace engine
{
    class Engine;
}

namespace editor
{
    class RigidBodyStaticComponent : public BaseComponent
    {
    public :

        /// Constructor
        RigidBodyStaticComponent(void);

        /// Destructor
        ~RigidBodyStaticComponent(void);

    protected :

        virtual void SectionContent(void) override;

    private :

        /// Functions
        void            UpdateShapeGeometry(int32 inCurrentIndex, engine::EGeometryType inGeometryType,
                                            engine::RigidBodyStatic* inRigidBodyStatic);
        void            DisplayUI(engine::EGeometryType inGeometryType,
                                  engine::RigidBodyStatic* inRigidBodyStatic);

        /// Private members
        std::vector<const char*> m_shapeTypes = { "Box", "Sphere", "Capsule", "Plane" };
        std::string m_geometryName;
        bool m_isInitialized = false;

    }; // !Class RigidBodyStaticComponent
} // !Namespace editor