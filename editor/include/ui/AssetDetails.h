#pragma once

#include <engine/resource/Resource.h>
#include <engine/ui/UIWindow.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/utility/ResourceRef.h>

#include <string>

namespace engine
{
    class MeshMaterial;
    class IResource;
}


namespace editor
{
    class AssetDetailsWnd final : public ::ui::UIWindow
    {

    private:

        struct MeshMaterialData
        {
            std::string m_mapNames[5]{};
        };

        struct AnimData
        {
            std::string    m_animName;
            math::Quatf    m_rotation;
            math::Vector3f m_position;
            math::Vector3f m_scale;
            math::Vector3f m_rotationEuler;
            math::Vector3f m_color;
        };

    public:

        enum class EAssetType
        {
            INVALID,
            SCENE,
            SCRIPT,
            MATERIAL,
            ANIMATION,
            MODEL
        };



        AssetDetailsWnd(void);
        AssetDetailsWnd(const AssetDetailsWnd&) = delete;
        AssetDetailsWnd(AssetDetailsWnd&&) noexcept = default;
        ~AssetDetailsWnd(void);

        void SelectAsset(const std::string& path, EAssetType type);
        void SetVisualizer(class ResourceVisualizerWnd* window);

    private:

        void RenderContents(void) override;

        void InitAnimationData(const std::string& name);
        void RenderAnimationWindow(void);
        void RenderAnimationInputWindow(void);
        void RenderAnimationTransformWindow(void);
        void RenderAnimationPositionInput(void);
        void RenderAnimationRotationInput(void);
        void RenderAnimationScaleInput(void);

        void RenderMaterialWindow(void);
        void RenderMaterialTextureInput(engine::MeshMaterial* material);
        void RenderMaterialColorInput(engine::MeshMaterial* material);
        void RenderMaterialInputBoxes(engine::MeshMaterial* material);
        void RenderMaterialSaveButton(engine::MeshMaterial* material);
        std::string GetTextureName(uint64 index);

        void ResetData(void);
        void DestroyMaterialData(void);
        void InitMaterialData(void);

        engine::EditableRef<engine::IResource> m_selectedResource;

        union
        {
            MeshMaterialData m_matData;
            AnimData         m_animData;
        };

        class ResourceVisualizerWnd*    m_visualizer;
        EAssetType                      m_selectedAssetType = EAssetType::INVALID;

    };
}