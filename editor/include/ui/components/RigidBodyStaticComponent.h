#pragma once

#pragma region Editor

#include "Component.h"

#pragma endregion

#pragma region Engine

#include <engine/CoreTypes.h>

#pragma endregion

#pragma region Standard

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

        std::string m_geometryName;
        engine::Engine* m_engine;

    }; // !Class RigidBodyStaticComponent
} // !Namespace editor