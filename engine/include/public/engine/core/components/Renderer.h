#pragma once

#include "engine/EngineExport.h"
#include "engine/core/Component.h"
#include "engine/core/ComponentArray.hpp"
#include "engine/core/components/Transform.h"
#include "engine/resource/animation/SkeletonAnimator.h"
#include "engine/utility/ResourceRefDecl.h"

#include <math/Matrix4.hpp>
#include <fstream>

namespace engine
{
    class Renderer final : public Component
    {
    private:

        using MaterialArray = std::vector<ResourceRef<class MeshMaterial>>;

    public:

        ENGINE_API
        Renderer(const Renderer&) = default;

        ENGINE_API
        Renderer(EntityHandle owner, class SceneGraph* scene);

        ENGINE_API
        Renderer(Renderer&&) = default;

        ENGINE_API
        ~Renderer(void) = default;

        ENGINE_API
        void Register(void) override;

        ENGINE_API
        void Unregister(void) override;

        ENGINE_API
        void Render(const math::Matrix4f& viewProjection,
                    ComponentArray<Transform>& transforms);

        void Render(const math::Matrix4f& view, const math::Matrix4f& projection,
            ComponentArray<class Transform>& transforms);

        ENGINE_API
        const ResourceRef<class Model>& GetModel(void) const;

        ENGINE_API
        const ResourceRef<class ShaderProgram>& GetShader(void) const;

        ENGINE_API
        const ResourceRef<class MeshMaterial>& GetMaterial(uint32 index) const;

        ENGINE_API
        uint32 GetMaterialCount(void) const;

        ENGINE_API
        void SetMaterial(uint32 index, ResourceRef<class MeshMaterial>&& material);

        ENGINE_API
        void SetMaterial(uint32 index, const char* key);
        
        ENGINE_API
        void SetModel(ResourceRef<class Model>&& model);

        ENGINE_API
        void SetModel(const char* key);

        ENGINE_API
        void SetShader(ResourceRef<class ShaderProgram>&& shader);

        ENGINE_API
        void SetShader(const char* key);

        ENGINE_API
        void SerializeText(std::ostream& output,
                           EntityHandle owner,
                           uint64 index) const;

       ENGINE_API
       SkeletonAnimator& GetAnimator(void);

        ENGINE_API
        const char* DeserializeText(const char* text, const char* end) override;

        ENGINE_API
        Renderer& operator=(const Renderer&) = default;

        ENGINE_API
        Renderer& operator=(Renderer&&) noexcept = default;

    private:

        SkeletonAnimator                    m_animator;
        MaterialArray                       m_materials;
        ResourceRef<class Model>            m_model;
        ResourceRef<class ShaderProgram>    m_shader;
    };


    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<Renderer>()
    {
        return RENDERER;
    }

}