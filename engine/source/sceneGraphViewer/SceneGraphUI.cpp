#include "sceneGraphViewer/SceneGraphUI.h"
#include "core/SceneGraph.h"
#include "input/Input.h"

#include "ui/UIWindow.h"
#include "ui/InternalUIDraw.h"
#include "ui/InternalUIWindow.h"
#include "ui/InternalUIDragDrop.h"
#include "ui/InternalUITree.h"
#include "ui/InternalUIComponent.h"
#include "ui/InternalUIStyle.h"

#include "utility/MemoryCheck.h"

#undef new
//#include <imgui/imgui.h>

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
    : m_graph(nullptr), m_reset(false)
{
    SetName(title);
    SetFlags(EWndFlags::HORIZONTAL_SCROLL_BAR | EWndFlags::NO_COLLAPSE | EWndFlags::MENU_BAR);
    InitRootNode();

    Input::RegisterInput(MOUSE_BUTTON_RIGHT);
    Input::RegisterInput(KEY_ENTER);
}

engine::SceneGraphViewer::SceneGraphViewer(const char* title, SceneGraph* graph)
    : m_graph(graph), m_reset(false)
{
    SetName(title);
    SetFlags(EWndFlags::HORIZONTAL_SCROLL_BAR | EWndFlags::NO_COLLAPSE | EWndFlags::MENU_BAR);
    InitRootNode();
    CreateGraph();

    Input::RegisterInput(MOUSE_BUTTON_RIGHT);
    Input::RegisterInput(KEY_ENTER);
}

engine::SceneGraphViewer::~SceneGraphViewer(void)
{
    DeleteGraph();
}

void engine::SceneGraphViewer::SetGraph(SceneGraph* graph)
{
    m_graph = graph;
    
    if (m_root)
        DeleteGraph();

    InitRootNode();
    CreateGraph();
}

void engine::SceneGraphViewer::RenderContents(void)
{
    RenderMenuBar();

    if (!m_graph)
        return;
    
    for (TreeNode* node : m_root->m_children)
    {
        DrawCurrentAndChildrenNodes(node);

        if (m_reset)
        {
            m_reset = false;
            break;
        }
    }
}

void engine::SceneGraphViewer::InitRootNode(void)
{
    m_root = new TreeNode();
    m_root->Init(nullptr, Entity::EHandleUtils::INVALID_HANDLE);
}

void engine::SceneGraphViewer::AddNode(TreeNode* parentNode)
{
    // Create new entity
    int32 id = 0;
    std::string newEntityName("New Entity ");
    char idStr[5];

    while (1)
    {
        _itoa_s(id++, idStr, 10);

        if (!m_graph->GetEntity(newEntityName + std::string(idStr)))
            break;
    }

    newEntityName += std::string(idStr);
    EntityHandle handle = m_graph->CreateEntity(newEntityName);

    // Create new node
    TreeNode* newNode = new TreeNode();
    newNode->Init(m_root, handle);
    parentNode->m_children.emplace_back(newNode);
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
    while (!node->m_children.empty())
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

/*
    TODO:
    - Add ability to create new node from right click menu
    - Don't delete recursively (reparent children to deleted node's parent)
    - Encapsulate
    - Clean up
*/
void engine::SceneGraphViewer::DrawCurrentAndChildrenNodes(TreeNode* node)
{
    int32 dragDropFlags = (node->m_handle == m_renamingHandle) ? ui::EDragDropSrcFlags::ALLOW_NULL_ID : 0;
    int32 nodeFlags =
        ui::SPAN_FULL_WIDTH | ui::FRAME_PADDING |
        ui::OPEN_ON_DOUBLE_CLICK | ui::OPEN_WITH_ARROW;

    if (node && node->m_children.empty())
        nodeFlags |= ui::NO_ICON;

    std::string entityName = m_graph->GetEntity(node->m_handle)->GetName();

    
    ImGui::PushID(entityName.c_str());
    bool result = ui::TreeNode((node->m_handle == m_renamingHandle) ? std::string("###") + entityName : entityName, nodeFlags);

    if (ImGui::BeginPopupContextItem("Options"))
    {
        if (ImGui::MenuItem("Add Entity"))
            AddNode(node);

        if (ImGui::MenuItem("Rename"))
        {
            //enableRename = true;
            m_renamingHandle = node->m_handle;
            printf("Renaming %s...\n", entityName.c_str());
        }

        if (ImGui::MenuItem("Delete"))
        {
            DeleteAllChildren(node);

            TreeNode* parentNode = node->m_parent;
            for (uint64 childIndex = 0; childIndex < parentNode->m_children.size(); ++childIndex)
            {
                if (parentNode->m_children[childIndex]->m_handle == node->m_handle)
                {
                    delete node;
                    parentNode->m_children.erase(parentNode->m_children.begin() + childIndex);
                    break;
                }
            }

            m_graph->DestroyEntity(node->m_handle);

            printf("Deleting %s...\n", entityName.c_str());
        }
        ImGui::EndPopup();
    }

    if (m_renamingHandle == node->m_handle)
    {
        ImGui::SameLine();
        constexpr const uint16 bufferLength = 128;
        char renameBuffer[bufferLength] = "\0";
        ImGui::SetKeyboardFocusHere();
        if (ImGui::InputTextWithHint("##rename", entityName.c_str(), renameBuffer, bufferLength, ImGuiInputTextFlags_EnterReturnsTrue) ||
            ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Middle) || ImGui::IsMouseDown(ImGuiMouseButton_Right))
        {
            if (renameBuffer[0] != '\0' && !m_graph->GetEntity(renameBuffer))
                m_graph->GetEntity(m_renamingHandle)->SetName(renameBuffer);

            m_renamingHandle = 0;
        }
        else
            m_renamingHandle = node->m_handle;
    }
    ImGui::PopID();

    if (ui::StartDragDropSource(dragDropFlags))
    {
        ui::CreatePayload("Entity", node, sizeof(TreeNode));
        ui::Text("%s", entityName.c_str());
        ui::EndDragDropSource();
    }

    if (result)
    {
        if (DragDropNode("Entity", node, 0) ||
            DragDropBackground("Entity", 0))
        {
            ui::EndTreeNode();
            ui::EndDragDropTarget();

            m_reset = true;
            return;
        }
    

        for (TreeNode* childNode : node->m_children)
        {
            if (m_reset)
                break;

            DrawCurrentAndChildrenNodes(childNode);
        }
        ui::EndTreeNode();
    }
    
    
}

const engine::ui::Payload engine::SceneGraphViewer::DragDropNode(const char* payloadID, TreeNode* node, int32 flags)
{
    if (ui::StartDragDropTarget())
    {
        if (ui::Payload payload = ui::AcceptPayload(payloadID, flags))
        {
            TreeNode* selectedNode = reinterpret_cast<TreeNode*>(payload.GetData());
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

        ui::EndDragDropTarget();
    }

    return nullptr;
}

const engine::ui::Payload engine::SceneGraphViewer::DragDropBackground(const char* payloadID, int32 flags)
{
    if (ui::StartDragDropTargetCustom(GetWindowRect(), m_title.c_str()))
    {
        if (ui::Payload payload = ui::AcceptPayload(payloadID, flags))
        {
            if (payload.IsPreview())
            {
                ui::DrawList drawList = ui::GetForegroundDrawList();
                const ui::UIWindowRect transform = ui::GetCurrentWindowRect();
                uint32 color = ::ui::GetColor(::ui::DRAG_DROP_TARGET_COLOR, 0.05f);
                drawList.AddRectFilled(transform.m_min, transform.m_max, color);
            }

            if (payload.IsDelivery())
            {
                TreeNode* selectedNode = reinterpret_cast<TreeNode*>(payload.GetData());
                ReparentNode(selectedNode, m_root);

                return payload;
            }
        }
        ui::EndDragDropTarget();
    }

    return nullptr;
}

void engine::SceneGraphViewer::RenderMenuBar(void)
{
    ui::StartDisabledSection(!m_graph);
    if (ui::StartMenuBar())
    {
        if (ui::Button("Add Entity"))
            AddNode(m_root);
        
        ui::EndMenuBar();
    }
    ui::EndDisabledSection();
}

void engine::SceneGraphViewer::RenderPopupMenu(void)
{
    if (ImGui::BeginPopup("Options"))
    {

        ImGui::EndPopup();
    }
}


