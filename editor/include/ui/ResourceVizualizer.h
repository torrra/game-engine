#pragma once

#include <engine/resource/Resource.h>
#include <engine/resource/animation/SkeletonAnimator.h>
#include <engine/resource/model/Model.h>

#include <engine/ui/UIWindow.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/utility/ResourceRef.h>

#include "EditorCamera.h"
#include "AssetDetails.h"

namespace editor
{

    class ResourceVisualizerWnd final : public ::ui::UIWindow
    {
    
    public:

        ResourceVisualizerWnd(void);
        ResourceVisualizerWnd(const ResourceVisualizerWnd&) = delete;
        ResourceVisualizerWnd(ResourceVisualizerWnd&&) = default;
        ~ResourceVisualizerWnd(void);


        void SelectResource(const engine::ResourceRef<engine::IResource>& resource,
                            AssetDetailsWnd::EAssetType type);

        void SetAnimKeyFrame(int32 frame);
        void SetAnimSpeed(f32 speed);
        void PauseAnim(void);
        void PlayAnim(void);

    private:

        void RenderContents(void) override;

        class Viewport*             m_viewport = nullptr;
        engine::SkeletonAnimator    m_animator;
        EditorCamera                m_camera;

        AssetDetailsWnd::EAssetType            m_resourceType = AssetDetailsWnd::EAssetType::INVALID;
        engine::ResourceRef<engine::IResource> m_resource;
        engine::ResourceRef<engine::Model>     m_displayModel;
    };
}
