#pragma once

#include <engine/CoreTypes.h>
#include <engine/ui/UIWindow.h>
#include <engine/ui/UITable.h>
#include <engine/ui/UIList.h>
#include <filesystem>
#include <vector>
#include <string>

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
        Asset(std::filesystem::path const& path);

        std::filesystem::path m_path;
        std::string m_fileName;
    };

    class AssetsWnd : public ::ui::UIWindow
    {
    public:
        AssetsWnd(void) = delete;
        AssetsWnd(const char* name);
        ~AssetsWnd(void);

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
        bool IsSupportedExtension(std::string const& extension);
        void RenderFile(std::filesystem::path const& file);

        std::vector<Asset> m_assets;
        DirTreeNode* m_rootNode;
        DirTreeNode* m_selectedDirectory;
        ui::Table* m_layout;
        ui::ListClipper m_clipper;
        int16 m_selectedIndex;
    };
}