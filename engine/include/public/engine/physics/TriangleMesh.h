#pragma once

#pragma region Core

#include "engine/core/Component.h"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"

#pragma endregion

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

namespace engine
{
    class Model;
    struct TriangleMeshImpl;

    class TriangleMesh : public Component
    {
    public :

        TriangleMesh(void) = delete;

        ENGINE_API TriangleMesh(EntityHandle inOwner, class SceneGraph* inScene);
        ENGINE_API void physicsTest(void);

        ENGINE_API TriangleMeshImpl* GetTriangleMeshImpl(void) { return m_triangleMeshImpl; }

        ENGINE_API void Register(void) override {}

    private :

        TriangleMeshImpl* m_triangleMeshImpl = nullptr;
        const Model* m_model = nullptr;

    }; // !Class TriangleMesh
} // !Namespace engine