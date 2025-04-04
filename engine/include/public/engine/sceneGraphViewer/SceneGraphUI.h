#pragma once

#include "engine/CoreTypes.h"
#include "engine/core/Entity.h"

#include <string>
#include <vector>

struct ImGuiPayload;

namespace engine
{
    struct TreeNode
    {
        ~TreeNode(void);

        std::vector<TreeNode*> m_children;
        TreeNode* m_parent;
        EntityHandle m_handle;
    };

    class SceneGraphViewer
    {
    public:
        SceneGraphViewer(void) = delete;
        SceneGraphViewer(const char* title);
        SceneGraphViewer(const char* title, class SceneGraph* graph);
        ~SceneGraphViewer(void);

        void DrawGraph(void);
        void SetGraph(class SceneGraph* graph);

    private:
        void InitRootNode(void);
        void CreateGraph(void);
        void DeleteAllChildren(TreeNode* node);
        void DeleteGraph(void);
        void ReparentNode(TreeNode* toReparent, TreeNode* newParent);
        void DrawCurrentAndChildrenNodes(TreeNode* node);
        const ImGuiPayload* DragDropBackground(const char* payloadID);

        std::string m_title;
        class SceneGraph* m_graph;
        TreeNode* m_root;
        bool m_reset = false;
    };
}