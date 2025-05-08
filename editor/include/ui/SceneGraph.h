#pragma once

#include <engine/CoreTypes.h>
#include <engine/core/Entity.h>
#include <engine/ui/UIWindow.h>

#include <string>
#include <vector>

namespace ui
{
    class Payload;
}

namespace editor
{
    struct TreeNode
    {
        ~TreeNode(void);

        void Init(TreeNode* parent, engine::EntityHandle handle);

        std::vector<TreeNode*> m_children;
        TreeNode* m_parent;
        engine::EntityHandle m_handle;
    };

    class SceneGraphUI : public ::ui::UIWindow
    {
    public:
        SceneGraphUI(void) = delete;
        SceneGraphUI(const char* title);
        SceneGraphUI(const char* title, class engine::SceneGraph* graph);
        ~SceneGraphUI(void);

        // Clear the graph for later re-use and reset root node
        void ClearGraph(void);

        // Scene engine scene graph to be displayed and operated on
        void SetGraph(class engine::SceneGraph* graph);
        void SelectEntity(engine::EntityHandle const& handle);

        engine::EntityHandle GetSelectedEntity(void) const noexcept;
        bool IsNewEntitySelected(void) const noexcept;
    
    protected:
        virtual void RenderContents(void) override;
    
    private:
        void InitRootNode(void);
        void AddNode(TreeNode* parentNode);
        void CreateGraph(void);
        void DeleteAllChildren(TreeNode* node);

        // Fully empty graph without root node reallocation
        void DeleteGraph(void);

        void ReparentNode(TreeNode* toReparent, TreeNode* newParent);
        void CheckRenameNode(TreeNode* node, std::string const& originalName);
        void DrawNodeAndChildren(TreeNode* node);

        const ::ui::Payload DragDropNode(const char* payloadID, TreeNode* node, int32 flags);
        const ::ui::Payload DragDropBackground(const char* payloadID, int32 flags);

        int32 GetNodeFlags(TreeNode* node) const;
        int32 GetDragDropFlags(int64 const nodeHandle) const;

        void RenderMenuBar(void);
        void RenderPopupMenu(TreeNode* node);

        class engine::SceneGraph* m_graph;
        TreeNode* m_root;
        int64 m_renamingHandle;
        int64 m_selectedHandle;
        bool m_reset = false;
        bool m_newEntitySelected = false;
    };
}