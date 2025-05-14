#pragma once

#pragma region Editor

#include "Component.h"

#pragma endregion

#pragma region Engine

#include <engine/CoreTypes.h>
#include <engine/physics/geometry/Geometry.hpp>
#include <engine/physics/rigidbody/RigidBodyDynamic.h>

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
    class RigidBodyDynamicComponent : public BaseComponent
    {
    public:

        /// Constructor
                        RigidBodyDynamicComponent(void);

        /// Destructor
                        ~RigidBodyDynamicComponent(void);

    protected:

        virtual void    SectionContent(void) override;

    private:

        /// Functions
        void            UpdateShapeGeometry(int inCurrentIndex, engine::EGeometryType inGeometryType,
                                            std::vector<const char*> inShapeTypes,
                                            engine::RigidBodyDynamic* inRigidBodyDynamic);
        void            DisplayUI(engine::EGeometryType inGeometryType,
                                  engine::RigidBodyDynamic* inRigidBodyDynamic);

        /// Private members
        std::string m_geometryName;
        engine::Engine* m_engine;

    }; // !Class RigidBodyDynamicComponent
} // !Namespace editor