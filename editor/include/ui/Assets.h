#pragma once

#include <engine/CoreTypes.h>
#include <engine/ui/UIWindow.h>
#include <engine/ui/UITable.h>
#include <filesystem>
#include <vector>
#include <string>

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

    class AssetsWnd : public ::ui::UIWindow
    {
    public:
        AssetsWnd(void) = delete;
        AssetsWnd(const char* name);
        ~AssetsWnd(void);

    protected:
        virtual void RenderContents(void) override;

    private:
        void RenderDirectorySection(math::Vector2f const& windowSize);
        void RenderDirectories(DirTreeNode* node);
        DirTreeNode* InitDirectoryRecursively(std::filesystem::path const& path, DirTreeNode* parentNode = nullptr);
        int32 GetTreeNodeFlags(DirTreeNode* node);

        DirTreeNode* m_rootNode;
        DirTreeNode* m_selectedNode;
        ui::Table* m_layout;
        bool m_newNodeSelected;
    };
}