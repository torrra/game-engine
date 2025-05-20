#pragma once

#pragma region Editor

#include "Component.h"

#pragma endregion

#pragma region Engine

#include <engine/CoreTypes.h>
#include <engine/physics/geometry/Geometry.hpp>
#include <engine/physics/rigidbody/RigidBodyDynamic.h>
#include <engine/CoreTypes.h>

#pragma endregion

#pragma region Standard

#include <vector>
#include <string>

#pragma endregion

namespace editor
{
    class RigidBodyDynamicComponent : public BaseComponent
    {
    public:

        /// Constructor
                        RigidBodyDynamicComponent(void);

        /// Destructor
                        ~RigidBodyDynamicComponent(void);

        void            SetData(engine::SceneGraph* graph, engine::EntityHandle owner) override;

    protected:

        virtual void    SectionContent(void) override;

    private:

        /// Functions
        void            UpdateShapeGeometry(int32 inCurrentIndex, 
                                            engine::EGeometryType inGeometryType,
                                            engine::RigidBodyDynamic* inRigidBodyDynamic);
        void            DisplayUI(engine::EGeometryType inGeometryType,
                                  engine::RigidBodyDynamic* inRigidBodyDynamic);
        void            GravityUI(engine::RigidBodyDynamic* inRigidBodyDynamic);
        void            TriggerUI(engine::RigidBodyDynamic* inRigidBodyDynamic);
        void            LockAxisUI(engine::RigidBodyDynamic* inRigidBodyDynamic);

        /// Private members
        std::vector<const char*> m_shapeTypes = { "Box", "Sphere", "Capsule" };
        std::string m_geometryName;
        bool m_isClosed = false;

    }; // !Class RigidBodyDynamicComponent
} // !Namespace editor