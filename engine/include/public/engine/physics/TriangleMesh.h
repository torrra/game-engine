#pragma once

#pragma region Core

#include "engine/core/Component.h"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/geometry/Geometry.hpp"
#include "engine/physics/ICollisionListener.h"

#pragma endregion

#pragma region Engine

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

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
        ENGINE_API                      ~TriangleMesh(void) override {};

        /// Getters
        // Get the triangle mesh implementation pointer
        ENGINE_API TriangleMeshImpl*    GetTriangleMeshImpl(void);

        /// Functions
        // Create a triangle mesh in regard to the model of the entity
        ENGINE_API void                 CreateTriangleMesh(void);
        
        ENGINE_API void                 Register(void) override {}

        ENGINE_API void				    SerializeText(std::ostream& output,
                                                      EntityHandle owner,
                                                      uint64 index) const override;
        ENGINE_API const char*          DeserializeText(const char* text, const char* end) override;
    private :

        /// Functions
        // Cook the triangle mesh
        void                            CookTriangleMesh(void);

        /// Private members
        const   Model*              m_model             = nullptr;
                TriangleMeshImpl*   m_triangleMeshImpl  = nullptr;

    }; // !Class TriangleMesh
} // !Namespace engine