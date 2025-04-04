#include "sceneGraphViewer/SceneGraphUI.h"
#include "core/SceneGraph.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "utility/MemoryCheck.h"

// Node Functions
engine::TreeNode::~TreeNode(void)
{
    m_children.clear();
    m_handle = 0;
    m_parent = nullptr;
}

void engine::TreeNode::Init(TreeNode* parent, EntityHandle handle)
{
    m_children = std::vector<TreeNode*>();
    m_parent = parent;
    m_handle = handle;
}

// Graph Window Functions
engine::SceneGraphViewer::SceneGraphViewer(const char* title)
    : m_title(title), m_graph(nullptr), m_reset(false)
{
    InitRootNode();
}

engine::SceneGraphViewer::SceneGraphViewer(const char* title, SceneGraph* graph)
    : m_title(title), m_graph(graph), m_reset(false)
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
    m_root = new TreeNode();
    m_root->Init(nullptr, Entity::EHandleUtils::INVALID_HANDLE);
}

void engine::SceneGraphViewer::CreateGraph(void)
{
    std::vector<EntityHandle> handles = m_graph->GetChildrenAllLevels(m_root->m_handle);
    std::vector<TreeNode*> nodes;

    for (uint64 handleIndex = 0; handleIndex < handles.size(); ++handleIndex)
    {
        TreeNode* newNode = new TreeNode();
        EntityHandle parentHandle = m_graph->GetEntity(handles[handleIndex])->GetParent();

        // Find parent node
        if (parentHandle != Entity::EHandleUtils::INVALID_HANDLE)
        {
            for (TreeNode* parentNode : nodes)
            {
                if (parentNode->m_handle != parentHandle)
                    continue;
                
                newNode->Init(parentNode, handles[handleIndex]);
                parentNode->m_children.emplace_back(newNode);

                break;
            }
        }
        else
        {
            // Root level
            newNode->Init(m_root, handles[handleIndex]);
            m_root->m_children.emplace_back(newNode);
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

void engine::SceneGraphViewer::ReparentNode(TreeNode* toReparent, TreeNode* newParent)
{
    TreeNode* prevParent = toReparent->m_parent;

    // Find node
    uint64 childIndex = 0;
    for (; childIndex < prevParent->m_children.size(); ++childIndex)
    {
        if (prevParent->m_children[childIndex]->m_handle == toReparent->m_handle)
            break;
    }

    m_graph->ReparentEntity(prevParent->m_children[childIndex]->m_handle, newParent->m_handle);
    prevParent->m_children[childIndex]->m_parent = newParent;
    newParent->m_children.emplace_back(prevParent->m_children[childIndex]);
    prevParent->m_children.erase(prevParent->m_children.begin() + childIndex);
}

void engine::SceneGraphViewer::DrawCurrentAndChildrenNodes(TreeNode* node)
{
    static constexpr ImGuiDragDropFlags dragDropFlags = ImGuiDragDropFlags_PayloadNoCrossContext;
    ImGuiTreeNodeFlags nodeFlags =
        ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding |
        ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

    if (node && node->m_children.empty())
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    std::string entityName = m_graph->GetEntity(node->m_handle)->GetName();
    if (ImGui::TreeNodeEx(entityName.c_str(), nodeFlags))
    {
        if (ImGui::BeginDragDropSource(dragDropFlags))
        {
            ImGui::SetDragDropPayload("Entity", node, sizeof(TreeNode));
            ImGui::Text("%s", entityName.c_str());
            ImGui::EndDragDropSource();
        }

        if (DragDropNode("Entity", node, 0) ||
            DragDropBackground("Entity", 0))
        {
            ImGui::TreePop();
            ImGui::EndDragDropTarget();

            m_reset = true;
            return;
        }

        for (TreeNode* childNode : node->m_children)
        {
            if (m_reset)
                break;

            DrawCurrentAndChildrenNodes(childNode);
        }
        ImGui::TreePop();
    }
}

const ImGuiPayload* engine::SceneGraphViewer::DragDropNode(const char* payloadID, TreeNode* node, int32 flags)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadID, flags))
        {
            TreeNode* selectedNode = reinterpret_cast<TreeNode*>(payload->Data);
            TreeNode* prevParent = node->m_parent;

            bool blockAction = false;

            while (prevParent->m_handle != Entity::EHandleUtils::INVALID_HANDLE)
            {
                if (prevParent->m_handle == selectedNode->m_handle)
                {
                    blockAction = true;
                    break;
                }
                else
                    prevParent = prevParent->m_parent;
            }

            if (!blockAction)
                ReparentNode(selectedNode, node);

            return payload;
        }

        ImGui::EndDragDropTarget();
    }

    return nullptr;
}

const ImGuiPayload* engine::SceneGraphViewer::DragDropBackground(const char* payloadID, int32 flags)
{
    ImRect transform = ImGui::GetCurrentWindow()->Rect();
    if (ImGui::BeginDragDropTargetCustom(transform, ImGui::GetID(m_title.c_str())))
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadID, flags))
        {
            if (payload->IsPreview())
            {
                ImDrawList* drawList = ImGui::GetForegroundDrawList();
                drawList->AddRectFilled(transform.Min, transform.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));
            }

            if (payload->IsDelivery())
            {
                TreeNode* selectedNode = reinterpret_cast<TreeNode*>(payload->Data);
                ReparentNode(selectedNode, m_root);

                return payload;
            }
        }
        ImGui::EndDragDropTarget();
    }

    return nullptr;
}
