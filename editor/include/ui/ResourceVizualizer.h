#pragma once

#include <engine/resource/Resource.h>
#include <engine/resource/animation/SkeletonAnimator.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/shader/Shader.h>

#include <engine/ui/UIWindow.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/utility/ResourceRef.h>

#include "EditorCamera.h"
#include "AssetDetails.h"

namespace editor
{

    class ResourceVisualizerWnd
    {
    
    public:

        ResourceVisualizerWnd(void);
        ResourceVisualizerWnd(const ResourceVisualizerWnd&) = delete;
        ResourceVisualizerWnd(ResourceVisualizerWnd&&) = default;
        ~ResourceVisualizerWnd(void);

        const engine::ResourceRef<engine::IResource>& GetResource(void) const;
        const engine::ResourceRef<engine::Model>& GetDisplayModel(void) const;

        void Init(void);
        void Update(f32 deltaTime);
        void Reset(void);
        engine::SkeletonAnimator& GetAnimator(void);

        void SelectAnimation(const engine::ResourceRef<class engine::Animation>& anim);
        void SetDisplayModel(engine::ResourceRef<engine::Model>&& model);

        void SetTransformMat(const math::Matrix4f& mat);
        void SetDisplayColor(const math::Vector3f& color);

    private:

        void RenderAnimation(f32 deltaTime);


        engine::SkeletonAnimator    m_animator;

        math::Matrix4f m_modelMat{ 1.f };

        engine::ResourceRef<engine::IResource>      m_resource;
        engine::ResourceRef<engine::Model>          m_displayModel;
        engine::ResourceRef<engine::ShaderProgram>  m_displayShader;

        EditorCamera                  m_camera;
        math::Vector3f                m_color{ 1.f };
        AssetDetailsWnd::EAssetType   m_resourceType = AssetDetailsWnd::EAssetType::INVALID;

        class Viewport*               m_viewport = nullptr;
    };
}
