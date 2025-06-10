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

    private:

        void RenderAnimation(f32 deltaTime);

        class Viewport*             m_viewport = nullptr;
        engine::SkeletonAnimator    m_animator;

        engine::ResourceRef<engine::IResource>      m_resource;
        engine::ResourceRef<engine::Model>          m_displayModel;
        engine::ResourceRef<engine::ShaderProgram>  m_displayShader;

        EditorCamera                m_camera;

        AssetDetailsWnd::EAssetType   m_resourceType = AssetDetailsWnd::EAssetType::INVALID;
    };
}
