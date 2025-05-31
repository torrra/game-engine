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
#include "engine/utility/ResourceRefDecl.h"

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

        ENGINE_API void                 CleanUpTriangleMesh(void);

        ENGINE_API void                 UpdateEntity(void);

        ENGINE_API void                 UpdateTriangleMesh(void);

    private :

        /// Friend class
        friend class TriangleMeshStaticFactory;

        /// Functions
        // Cook the triangle mesh
        void                            CookTriangleMesh(void);

        void                            SetCollisionGroupAndMask(uint32 inCollisionGroup,
                                                                 uint32 inCollisionMask);

        /// Private members
                ResourceRef<Model>          m_model;
                TriangleMeshImpl*           m_triangleMeshImpl  = nullptr;
                RigidBodyData               m_data;
                uint64                      m_shape             = 0;
                uint32                      m_type              = EShapeType::STATIC;
                collision::ECollisionGroup  m_collisionGroup    = 
                                                collision::ECollisionGroup::ENVIRONMENT_COLLISION;
                bool                        m_isDrawn           = false;

    }; // !Class TriangleMesh

    class TriangleMeshStaticFactory
    {
    public:

        /// Functions
        /*
            Fonction to create a dynamic rigid body
            <param> [in] inScene : the scene
            <param> [in] inOwner : the entity owner
            <param> [in] inGeometry : the geometry type
        */
        ENGINE_API static TriangleMesh* CreateStatic(class SceneGraph* inScene,
                                                     EntityHandle inOwner);
    }; // !Class RigidBodyStaticFactory

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<TriangleMesh>()
    {
        return TRIANGLE_MESH;
    }

} // !Namespace engine