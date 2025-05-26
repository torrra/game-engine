#pragma once

#include <engine/CoreTypes.h>
#include <engine/core/SceneGraph.h>
#include <engine/resource/shader/Shader.h>
#include <engine/utility/ResourceRef.h>
#include <math/Vector3.hpp>
#include <math/Matrix4.hpp>
#include <unordered_map>

namespace editor
{
    class PickableEntity
    {
    public:
        // Never use this constructor, it is only here due to the umap in the picking class
        PickableEntity(void) = default;
        PickableEntity(int64 const& handle);
        ~PickableEntity(void) = default;

        // Getters
        uint32 GetPickingID(void) const;
        int64 GetEntityHandle(void) const;
        math::Vector3f GetColor(void) const;

    private:
        int64 m_entityHandle;
        math::Vector3f m_color;
        uint32 m_pickingID;
    };


    class Picking
    {
    public:
        Picking(engine::SceneGraph* graph);
        ~Picking(void);

        engine::EntityHandle FindSelectedEntity(std::string const& wndName) const;
        void InitEntities(engine::SceneGraph* graph);
        void RenderSceneColored(engine::SceneGraph* graph, const math::Matrix4f& viewProjection);

    private:
        std::unordered_map<uint32, PickableEntity> m_pickableEntity;
        engine::ResourceRef<engine::ShaderProgram> m_pickingShader;
    };
}