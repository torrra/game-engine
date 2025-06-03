#pragma once

#pragma region Editor

#include "Component.h"

#pragma endregion

#pragma region Engine

#include <engine/CoreTypes.h>
#include <engine/physics/geometry/Geometry.hpp>
#include <engine/physics/TriangleMesh.h>
#include <engine/CoreTypes.h>

#pragma endregion

namespace engine
{
    class Engine;
}

namespace editor
{
    class TriangleMeshComponent : public BaseComponent
    {
    public :

        /// Constructor
        TriangleMeshComponent(void);

        /// Destructor
        ~TriangleMeshComponent(void);

    protected :

        virtual void SectionContent(void) override;

    private :

    }; // !Class TriangleMeshComponent
} // !Namespace editor