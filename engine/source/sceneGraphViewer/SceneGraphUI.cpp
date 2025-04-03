#include "sceneGraphViewer/SceneGraphUI.h"
#include "core/SceneGraph.h"

#undef new
#include <imgui/imgui.h>

#include "utility/MemoryCheck.h"

engine::SceneGraphViewer::SceneGraphViewer(const char* title)
    : m_title(title), m_graph(nullptr)
{
    InitRootNode();
}


engine::SceneGraphViewer::SceneGraphViewer(const char* title, SceneGraph* graph)
    : m_title(title), m_graph(graph)
{
    InitRootNode();
    CreateGraph();
}

engine::SceneGraphViewer::~SceneGraphViewer(void)
{
    DeleteGraph();
}

void engine::SceneGraphViewer::DrawGraph(void)
{
    if (!m_graph)
        return;

    // Window flags
    static const ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

    // TODO: add default docking node
    // TODO: change default size
    ImGui::SetNextWindowSize({300, 900}, ImGuiCond_Once);
    ImGui::Begin(m_title.c_str(), nullptr, flags);
    
    for (TreeNode* node : m_root->m_children)
    {
        DrawCurrentAndChildrenNodes(node);
        if (m_reset)
        {
            m_reset = false;
            break;
        }

    }

    ImGui::End();
}

void engine::SceneGraphViewer::SetGraph(SceneGraph* graph)
{
    m_graph = graph;
    
    if (m_root)
        DeleteGraph();

    InitRootNode();
    CreateGraph();
}

void engine::SceneGraphViewer::InitRootNode(void)
{
    m_root = new TreeNode
    {
        .m_children = std::vector<TreeNode*>(),
        .m_parent = nullptr,
        .m_handle = Entity::EHandleUtils::INVALID_HANDLE,
        .m_name = "root"
    };
}

void engine::SceneGraphViewer::AddNode(EntityHandle handle)
{
    Entity* entity = m_graph->GetEntity(handle);
    
    EntityHandle parentHandle =  entity->GetParent();
    TreeNode* parentNode = m_root;
    // Get parent node
    if (parentHandle != Entity::EHandleUtils::INVALID_HANDLE)
    {
        parentNode = FindNode(parentHandle, m_root);

        if (!parentNode)
            parentNode = m_root;
    }

    parentNode->m_children.push_back(new TreeNode
    {
        .m_children = std::vector<TreeNode*>(),
        .m_parent = parentNode,
        .m_handle = handle
    });
    printf("%s\n", m_graph->GetEntity(handle)->GetName().c_str());
}

engine::TreeNode* engine::SceneGraphViewer::FindNode(EntityHandle handle, TreeNode* root)
{
    if (!m_root)
        return nullptr;

    if (root->m_handle == handle)
        return root;

    for (TreeNode* childNode : root->m_children)
    {
        TreeNode* found = FindNode(handle, childNode);

        if (found)
        {
            return found;
        }
    }

    return nullptr;
    //for (TreeNode* treeNode : root->m_children)
    //{
    //    if (treeNode->m_handle == handle)
    //        return treeNode;
    //
    //    FindNode(handle, treeNode);
    //}
    //
    //return nullptr;
}

void engine::SceneGraphViewer::CreateGraph(void)
{
    std::vector<EntityHandle> handles = m_graph->GetChildrenAllLevels(m_root->m_handle);
    std::vector<TreeNode*> nodes;

    for (uint64 i = 0; i < handles.size(); ++i)
    {
        TreeNode* newNode = new TreeNode;

        EntityHandle parentHandle = m_graph->GetEntity(handles[i])->GetParent();

        // Find parent node
        if (parentHandle != Entity::EHandleUtils::INVALID_HANDLE)
        {
            for (TreeNode* parentNode : nodes)
            {
                if (parentNode->m_handle == parentHandle)
                {
                    newNode->m_children = std::vector<TreeNode*>();
                    newNode->m_parent = parentNode;
                    newNode->m_handle = handles[i];
                    newNode->m_name = m_graph->GetEntity(newNode->m_handle)->GetName().c_str();

                    parentNode->m_children.push_back(newNode);
                    break;
                }
            }
        }
        else
        {
            // Root
            newNode->m_children = std::vector<TreeNode*>();
            newNode->m_parent = m_root;
            newNode->m_handle = handles[i];
            newNode->m_name = m_graph->GetEntity(newNode->m_handle)->GetName().c_str();

            m_root->m_children.push_back(newNode);
        }

        nodes.push_back(newNode);
    }

    nodes.clear();
}

void engine::SceneGraphViewer::DeleteAllChildren(TreeNode* node)
{
    TreeNode* currentNode = node;
    while (!m_root->m_children.empty())
    {
        while (!currentNode->m_children.empty())
        {
            currentNode = currentNode->m_children[0];
        }
    
        currentNode = currentNode->m_parent;
        delete currentNode->m_children[0];
        currentNode->m_children.erase(currentNode->m_children.begin());
    }
}

void engine::SceneGraphViewer::DeleteGraph(void)
{
    DeleteAllChildren(m_root);

    if (m_root)
    {
        delete m_root;
        m_root = nullptr;
    }
}

void engine::SceneGraphViewer::DrawCurrentAndChildrenNodes(TreeNode* node)
{
    static constexpr ImGuiDragDropFlags dragDropFlags = 
        ImGuiDragDropFlags_PayloadNoCrossContext |
        ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
    
    ImGuiTreeNodeFlags nodeFlags =
        ImGuiTreeNodeFlags_SpanFullWidth |
        ImGuiTreeNodeFlags_FramePadding |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_OpenOnArrow;
    
    if (!node || !m_root)
        return;
    if (node && node->m_children.empty())
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    else
        nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen; // TODO: remove this flag (for debugging only)
    
   
    if (ImGui::TreeNodeEx(m_graph->GetEntity(node->m_handle)->GetName().c_str(), nodeFlags))
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("Entity", node, sizeof(TreeNode));
            ImGui::Text("%s", m_graph->GetEntity(node->m_handle)->GetName().c_str());
            ImGui::EndDragDropSource();
            printf("Dragging: %s\n", m_graph->GetEntity(node->m_handle)->GetName().c_str());
        }
        
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity", dragDropFlags) )
            {
                ImGui::TreePop();
                TreeNode* entity = reinterpret_cast<TreeNode*>(payload->Data);
                printf("Parent Entity: %s\n", m_graph->GetEntity(node->m_handle)->GetName().c_str());
                printf("Child Entity: %s\n", m_graph->GetEntity(entity->m_handle)->GetName().c_str());
                
                // New parent to node
                // node add entity to children
                // entity set parent to node
                
                // old parent remove entity from children


                //TreeNode* prevParent = entity->m_parent;
                //
                //node->m_children.push_back(entity);
                //entity->m_parent = node;
                //
                //for (uint64 index = 0; index < prevParent->m_children.size(); ++index)
                //{
                //    if (prevParent->m_children[index]->m_handle == entity->m_handle)
                //    {
                //        delete prevParent->m_children[index];
                //        prevParent->m_children.erase(prevParent->m_children.begin() + index);
                //        break;
                //    }
                //}

                m_graph->ReparentEntity(entity->m_handle, node->m_handle);
                node = m_root;
                SetGraph(m_graph);
                m_reset = true;
                ImGui::EndDragDropTarget();
                return;
            }
            ImGui::EndDragDropTarget();
        }

        for (TreeNode* childNode : node->m_children)
        {
            DrawCurrentAndChildrenNodes(childNode);
        }
    
        ImGui::TreePop();
    }


}

engine::TreeNode::~TreeNode(void)
{
    m_children.clear();
    m_handle = 0 << 0;
    m_parent = nullptr;
}
