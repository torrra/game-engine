#pragma once

#include "engine/CoreTypes.h"
#include "engine/core/Entity.h"

#include "engine/ui/UIWindow.h"

#include <string>
#include <vector>

namespace engine
{
    namespace ui
    {
        class Payload;
    }

    struct TreeNode
    {
        ~TreeNode(void);

        void Init(TreeNode* parent, EntityHandle handle);

        std::vector<TreeNode*> m_children;
        TreeNode* m_parent;
        EntityHandle m_handle;
    };

    class SceneGraphViewer : public UIWindow
    {
    public:
        SceneGraphViewer(void) = delete;
        SceneGraphViewer(const char* title);
        SceneGraphViewer(const char* title, class SceneGraph* graph);
        ~SceneGraphViewer(void);

        void SetGraph(class SceneGraph* graph);
    
    protected:
        virtual void RenderContents(void) override;
    
    private:
        void InitRootNode(void);
        void AddNode(TreeNode* parentNode);
        void CreateGraph(void);
        void DeleteAllChildren(TreeNode* node);
        void DeleteGraph(void);
        void ReparentNode(TreeNode* toReparent, TreeNode* newParent);
        void DrawCurrentAndChildrenNodes(TreeNode* node);
        
        const ui::Payload DragDropNode(const char* payloadID, TreeNode* node, int32 flags);
        const ui::Payload DragDropBackground(const char* payloadID, int32 flags);

        void RenderMenuBar(void);
        void RenderPopupMenu(void);

        class SceneGraph* m_graph;
        TreeNode* m_root;
        int64 m_renamingHandle;
        bool m_reset = false;
    };
}