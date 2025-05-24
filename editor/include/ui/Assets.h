#pragma once

#include <engine/CoreTypes.h>
#include <engine/ui/UIWindow.h>
#include <engine/ui/UITable.h>
#include <engine/ui/UIList.h>
#include <filesystem>
#include <vector>
#include <string>
#include <future>

/*
    -------- Supported extensions --------

    The extension listed below will be visible 
    inside the asset browser window.

    Models
    .OBJ
    .FBX
    .DAE

    Textures
    .PNG

    Fonts
    .TTF

    Scripts
    .LUA

    Shaders
    .VERT
    .FRAG

    Sound
    TODO: allow sound extensions
    --------------------------------------
*/

#define MODEL_PAYLOAD "model"
#define AUDIO_PAYLOAD "audio"
#define SCRIPT_PAYLOAD "script"
#define TEXTURE_PAYLOAD "texture"
#define FONT_PAYLOAD "font"
#define VERTEX_SHADER_PAYLOAD "vertexShader"
#define FRAGMENT_SHADER_PAYLOAD "fragmentShader"
#define MATERIAL_PAYLOAD "material"
#define SCENE_PAYLOAD "scene"

namespace editor
{
    struct DirTreeNode
    {
        DirTreeNode(std::filesystem::path const& path, DirTreeNode* parent);
        ~DirTreeNode(void);

        std::string m_dirName;
        std::filesystem::path m_path;
        std::vector<DirTreeNode*> m_children;
        DirTreeNode* m_parentNode;
    };

    struct Asset
    {
        Asset(void) = delete;
        Asset(std::filesystem::path const& path, std::string const& payloadType);

        std::filesystem::path m_path;
        std::string m_fileName;
        const std::string m_payloadType;
    };

    class AssetsWnd final : public ::ui::UIWindow
    {
    private:

        enum class EAssetAction
        {
            NONE,
            CREATE_SCENE,
            CREATE_MATERIAL,
            CREATE_SCRIPT,
            OPEN_ASSET,
            DELETE_ASSET,
            REFRESH_WINDOW
        };


    public:
        AssetsWnd(void) = delete;
        AssetsWnd(const char* name, class EditorApplication* owner);
        ~AssetsWnd(void);

        void SetPath(std::filesystem::path const& projectDir);
        
    protected:
        virtual void RenderContents(void) override;

    private:
        // Directory section
        void RenderDirectorySection(math::Vector2f const& windowSize);
        void RenderDirectories(DirTreeNode* node);
        DirTreeNode* InitDirectoryRecursively(std::filesystem::path const& path, DirTreeNode* parentNode = nullptr);
        int32 GetTreeNodeFlags(DirTreeNode* node);

        // Files section
        void RenderAssets(void);
        void OnSelectDir(void);
        bool IsSupportedExtension(std::string const& extension, std::string& payloadType);
        std::string GetPayloadType(std::string const& extension) const;

        void SelectResource(void);
        EAssetAction RenderRightClickMenu(void);
        EAssetAction CreateAsset(EAssetAction action);

        void SetNewAssetName(void);
        bool IsAssetNameValid(void);
        void CreateScene(void);
        void CreateMaterial(void);
        void CloseAssetCreationMenu(void);
        EAssetAction SelectNewAssetType(EAssetAction action);

        ui::Table   m_assetCreationTable = ui::Table("createAssetLayout", 3, {0.f, 0.f});
        std::string m_newAssetName;

        std::future<bool> m_isDirUpdated;
        std::filesystem::path m_path;
        std::vector<Asset> m_assets;
        DirTreeNode* m_rootNode;
        DirTreeNode* m_selectedDirectory;
        ui::Table* m_layout;

        class EditorApplication* m_ownerApplication;

        ui::ListClipper m_clipper;
        EAssetAction m_currentAction = EAssetAction::NONE;
        int16 m_selectedIndex;
        bool m_isRightClickMenuOpen = false;

    };
}