#pragma once

#include "engine/CoreTypes.h"
#include "engine/core/Entity.h"

#include <string>
#include <vector>

namespace engine
{
    struct TreeNode
    {
        ~TreeNode(void);

        std::vector<TreeNode*> m_children;
        TreeNode* m_parent;
        EntityHandle m_handle;
        std::string m_name;
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
        void AddNode(EntityHandle handle);
        TreeNode* FindNode(EntityHandle handle, TreeNode* root);
        void CreateGraph(void);
        void DeleteAllChildren(TreeNode* node);
        void DeleteGraph(void);
        void DrawCurrentAndChildrenNodes(TreeNode* node);

        std::string m_title;
        class SceneGraph* m_graph;
        TreeNode* m_root;
        bool m_reset = false;
    };

    //struct TreeNode
    //{
    //    std::vector<TreeNode*> m_children;
    //    EntityHandle m_parent;
    //    EntityHandle m_handle;
    //};
    //
    //class SceneGraphWnd
    //{
    //public:
    //    SceneGraphWnd(void) = delete;
    //    SceneGraphWnd(const char* title);
    //    SceneGraphWnd(const char* title, class SceneGraph* graph);
    //    ~SceneGraphWnd(void) = default;
    //
    //    void DrawWindow(void);
    //
    //    void SetGraph(class SceneGraph* graph);
    //private:
    //    void CreateNode(EntityHandle handle, SceneGraph* graph);
    //    void CreateRootNode(void);
    //    TreeNode* FindNode(EntityHandle nodeHandle, TreeNode* rootNode);
    //
    //    void UpdateGraph(void);
    //    void DrawNode(class Entity* entity);
    //
    //    std::string m_title;
    //    std::vector<EntityHandle> m_handles;
    //    class SceneGraph* m_graph;
    //    TreeNode* m_rootNode;
    //};
}