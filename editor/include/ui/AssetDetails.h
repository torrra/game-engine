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

    public:

        enum class EAssetType
        {
            INVALID,
            SCENE,
            SCRIPT,
            MATERIAL
        };



        AssetDetailsWnd(void);
        AssetDetailsWnd(const AssetDetailsWnd&) = delete;
        AssetDetailsWnd(AssetDetailsWnd&&) noexcept = default;
        ~AssetDetailsWnd(void);

        void SelectAsset(const std::string& path, EAssetType type);

    private:

        void RenderContents(void) override;


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
        };

        EAssetType         m_selectedAssetType = EAssetType::INVALID;

    };
}