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

        /// Constructor
        // Default constructor deleted
                                        TriangleMesh(void) = delete;
        // Component constructor
        ENGINE_API                      TriangleMesh(EntityHandle inOwner, class SceneGraph* inScene);

        /// Destructor
        ENGINE_API                      ~TriangleMesh(void);

        /// Getters
        // Get the triangle mesh implementation pointer
        ENGINE_API TriangleMeshImpl*    GetTriangleMeshImpl(void);

        /// Functions
        // Create a triangle mesh in regard to the model of the entity
        ENGINE_API void                 CreateTriangleMesh(void);
        
        ENGINE_API void                 Register(void) override {}

    private :

        /// Functions
        // Cook the triangle mesh
        void                            CookTriangleMesh(void);

        /// Private members
        const   Model*              m_model             = nullptr;
                TriangleMeshImpl*   m_triangleMeshImpl  = nullptr;

    }; // !Class TriangleMesh

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<TriangleMesh>()
    {
        return TRIANGLE_MESH;
    }

} // !Namespace engine