#include "sceneGraphViewer/SceneGraphUI.h"
#include "core/SceneGraph.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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
    };
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

void engine::SceneGraphViewer::ReparentNode(TreeNode* toReparent, TreeNode* newParent)
{
    TreeNode* prevParent = toReparent->m_parent;

    // Find node
    uint64 i = 0;
    for (; i < prevParent->m_children.size(); ++i)
    {
        if (prevParent->m_children[i]->m_handle == toReparent->m_handle)
            break;
    }

    m_graph->ReparentEntity(prevParent->m_children[i]->m_handle, newParent->m_handle);
    prevParent->m_children[i]->m_parent = newParent;
    newParent->m_children.emplace_back(prevParent->m_children[i]);
    prevParent->m_children.erase(prevParent->m_children.begin() + i);
}

void engine::SceneGraphViewer::DrawCurrentAndChildrenNodes(TreeNode* node)
{
    static constexpr ImGuiDragDropFlags dragDropFlags = 
        ImGuiDragDropFlags_PayloadNoCrossContext;
    
    ImGuiTreeNodeFlags nodeFlags =
        ImGuiTreeNodeFlags_SpanFullWidth |
        ImGuiTreeNodeFlags_FramePadding |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_OpenOnArrow;
    
    if (node && node->m_children.empty())
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    if (ImGui::TreeNodeEx(m_graph->GetEntity(node->m_handle)->GetName().c_str(), nodeFlags))
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("Entity", node, sizeof(TreeNode));
            ImGui::Text("%s", m_graph->GetEntity(node->m_handle)->GetName().c_str());
            ImGui::EndDragDropSource();
        }
        
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity", dragDropFlags) )
            {
                ImGui::TreePop();
                TreeNode* entity = reinterpret_cast<TreeNode*>(payload->Data);
                bool blockAction = false;
                TreeNode* prevParent = node->m_parent;
                while (prevParent->m_handle != Entity::EHandleUtils::INVALID_HANDLE)
                {
                    if (prevParent->m_handle == entity->m_handle)
                    {
                        blockAction = true;
                        break;
                    }
                    else
                        prevParent = prevParent->m_parent;
                }

                if (!blockAction)
                    ReparentNode(entity, node);
                
                m_reset = true;
            }

            ImGui::EndDragDropTarget();

            if (m_reset)
                return;
        }
        if (DragDropBackground("Entity"))
        {
            ImGui::TreePop();
            ImGui::EndDragDropTarget();

            m_reset = true;
            return;
        }

        for (TreeNode* childNode : node->m_children)
        {
            if (m_reset)
            {
                ImGui::TreePop();
                return;
            }

            DrawCurrentAndChildrenNodes(childNode);
        }
        
        ImGui::TreePop();
    }
}

const ImGuiPayload* engine::SceneGraphViewer::DragDropBackground(const char* payloadID)
{
    ImRect transform = ImGui::GetCurrentWindow()->Rect();
    if (ImGui::BeginDragDropTargetCustom(transform, ImGui::GetID(m_title.c_str())))
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadID, 0))
        {
            if (payload->IsPreview())
            {
                ImDrawList* drawList = ImGui::GetForegroundDrawList();
                drawList->AddRectFilled(transform.Min, transform.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));
                drawList->AddRect(transform.Min, transform.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.0f), 2.0f);
            }

            if (payload->IsDelivery())
            {
                TreeNode* entity = reinterpret_cast<TreeNode*>(payload->Data);
                ReparentNode(entity, m_root);

                return payload;
            }

        }
        ImGui::EndDragDropTarget();
    }

    return nullptr;
}

engine::TreeNode::~TreeNode(void)
{
    m_children.clear();
    m_handle = 0;
    m_parent = nullptr;
}
