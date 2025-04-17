#include "ui/SceneGraph.h"

#include <engine/core/SceneGraph.h>
#include <engine/ui/InternalUIWindow.h>
#include <engine/ui/UIDraw.h>
#include <engine/ui/UIDragDrop.h>
#include <engine/ui/UITree.h>
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIStyle.h>
#include <engine/input/Input.h>

#include <engine/utility/MemoryCheck.h>

// Node Functions
editor::TreeNode::~TreeNode(void)
{
    for (TreeNode* node : m_children)
        delete node;

    m_children.clear();
    m_handle = 0;
    m_parent = nullptr;
}

void editor::TreeNode::Init(TreeNode* parent, engine::EntityHandle handle)
{
    m_children = std::vector<TreeNode*>();
    m_parent = parent;
    m_handle = handle;
}

// Graph Window Functions
editor::SceneGraphUI::SceneGraphUI(const char* title)
    : m_graph(nullptr), m_reset(false), m_renamingHandle(0), m_selectedHandle(engine::Entity::EHandleUtils::INVALID_HANDLE)
{
    SetName(title);
    SetFlags(::ui::EWndFlags::HORIZONTAL_SCROLL_BAR | ::ui::EWndFlags::NO_COLLAPSE | ::ui::EWndFlags::MENU_BAR);
    InitRootNode();

    // TODO: register keys somewhere else
    engine::Input::RegisterInput(MOUSE_BUTTON_LEFT);
    engine::Input::RegisterInput(MOUSE_BUTTON_RIGHT);
}

editor::SceneGraphUI::SceneGraphUI(const char* title, engine::SceneGraph* graph)
    : m_graph(graph), m_reset(false), m_renamingHandle(0), m_selectedHandle(engine::Entity::EHandleUtils::INVALID_HANDLE)
{
    SetName(title);
    SetFlags(::ui::EWndFlags::HORIZONTAL_SCROLL_BAR | ::ui::EWndFlags::NO_COLLAPSE | ::ui::EWndFlags::MENU_BAR);
    
    if (graph)
    {
        InitRootNode();
        CreateGraph();
    }
    else
        m_root = nullptr;

    // TODO: register keys somewhere else
    engine::Input::RegisterInput(MOUSE_BUTTON_LEFT);
    engine::Input::RegisterInput(MOUSE_BUTTON_RIGHT);
}

editor::SceneGraphUI::~SceneGraphUI(void)
{
    DeleteGraph();
}

void editor::SceneGraphUI::SetGraph(engine::SceneGraph* graph)
{
    if (!graph)
        return;

    m_graph = graph;
    
    if (m_root)
        DeleteGraph();

    InitRootNode();
    CreateGraph();
}

void editor::SceneGraphUI::SelectEntity(engine::EntityHandle const& handle)
{
    m_selectedHandle = handle;
    m_newEntitySelected = true;
    m_reset = true;
}

engine::EntityHandle editor::SceneGraphUI::GetSelectedEntity(void) const noexcept
{
    return m_selectedHandle;
}

bool editor::SceneGraphUI::IsNewEntitySelected(void) const noexcept
{
    return m_newEntitySelected;
}

void editor::SceneGraphUI::RenderContents(void)
{
    RenderMenuBar();

    if (!m_graph)
        return;
    
    if (m_newEntitySelected)
        m_newEntitySelected = false;
    
    for (TreeNode* node : m_root->m_children)
    {
        DrawNodeAndChildren(node);

        if (m_reset)
        {
            m_reset = false;
            break;
        }
    }
}

void editor::SceneGraphUI::InitRootNode(void)
{
    m_root = new TreeNode();
    m_root->Init(nullptr, engine::Entity::EHandleUtils::INVALID_HANDLE);
}

void editor::SceneGraphUI::AddNode(TreeNode* parentNode)
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
    engine::EntityHandle handle = m_graph->CreateEntity(newEntityName);

    // Create new node
    TreeNode* newNode = new TreeNode();
    newNode->Init(parentNode, handle);
    parentNode->m_children.emplace_back(newNode);
}

void editor::SceneGraphUI::CreateGraph(void)
{
    std::vector<engine::EntityHandle> handles = m_graph->GetChildrenAllLevels(m_root->m_handle);
    std::vector<TreeNode*> nodes;

    for (uint64 handleIndex = 0; handleIndex < handles.size(); ++handleIndex)
    {
        TreeNode* newNode = new TreeNode;
        engine::EntityHandle parentHandle = m_graph->GetEntity(handles[handleIndex])->GetParent();

        // Find parent node
        if (parentHandle != engine::Entity::EHandleUtils::INVALID_HANDLE)
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

void editor::SceneGraphUI::DeleteAllChildren(TreeNode* node)
{
    // Don't delete the root node
    if (node->m_handle == m_root->m_handle)
        return;

    TreeNode* parentNode = node->m_parent;
    
    // Delete node & children
    delete node;

    // Delete node from parent's children array
    for (uint64 index = 0; index < parentNode->m_children.size(); ++index)
    {
        if (parentNode->m_children[index]->m_handle == node->m_handle)
        {
            parentNode->m_children.erase(parentNode->m_children.begin() + index);
            break;
        }
    }
}

void editor::SceneGraphUI::DeleteGraph(void)
{
    if (m_root)
    {
        DeleteAllChildren(m_root);
        
        delete m_root;
        m_root = nullptr;
    }
}

void editor::SceneGraphUI::ReparentNode(TreeNode* toReparent, TreeNode* newParent)
{
    TreeNode* prevParent = toReparent->m_parent;

    // Find node
    uint64 childIndex = 0;
    for (; childIndex < prevParent->m_children.size(); ++childIndex)
    {
        if (prevParent->m_children[childIndex]->m_handle == toReparent->m_handle)
            break;
    }

    // Reparent
    m_graph->ReparentEntity(prevParent->m_children[childIndex]->m_handle, newParent->m_handle);
    prevParent->m_children[childIndex]->m_parent = newParent;
    newParent->m_children.emplace_back(prevParent->m_children[childIndex]);
    prevParent->m_children.erase(prevParent->m_children.begin() + childIndex);
}

void editor::SceneGraphUI::CheckRenameNode(TreeNode* node, std::string const& originalName)
{
    if (m_renamingHandle != node->m_handle)
        return;

    std::string resultStr;

    // Prevent placing input text box on following line
    ::ui::SameLine();
    ::ui::SetKeyboardFocus();
    if (::ui::InputBox("##rename", originalName.c_str(), resultStr) ||
        engine::Input::IsInputDown(MOUSE_BUTTON_LEFT) || engine::Input::IsInputDown(MOUSE_BUTTON_RIGHT))
    {
        if (resultStr[0] != '\0' && !m_graph->GetEntity(resultStr))
            m_graph->GetEntity(m_renamingHandle)->SetName(resultStr);

        m_renamingHandle = 0;
    }
    else
        m_renamingHandle = node->m_handle;
}

void editor::SceneGraphUI::DrawNodeAndChildren(TreeNode* node)
{
    // Get entity & node name
    std::string entityName = m_graph->GetEntity(node->m_handle)->GetName();
    std::string nodeName = (node->m_handle == m_renamingHandle) ? ("###" + entityName) : entityName;

    ::ui::SetID(nodeName);
    // Set ID to prevent duplicate menu items

    bool isOpen = ::ui::TreeNode(nodeName, GetNodeFlags(node));

    if (ui::IsItemSelected())
    {
        m_selectedHandle = node->m_handle;
        m_newEntitySelected = true;
    }

    RenderPopupMenu(node);
    CheckRenameNode(node, entityName);
    ::ui::UnsetID();

    // Create drag & drop payload 
    if (::ui::StartDragDropSource(GetDragDropFlags(node->m_handle)))
    {
        ::ui::CreatePayload("Entity", node, sizeof(TreeNode));
        ::ui::Text("%s", entityName.c_str());
        ::ui::EndDragDropSource();
    }
    
    // Condition to check node state is open
    if (isOpen)
    {
        // Allow drop action for reparenting
        if (DragDropNode("Entity", node, 0) ||
            DragDropBackground("Entity", 0))
        {
            ::ui::EndTreeNode();
            ::ui::EndDragDropTarget();

            m_reset = true;
            return;
        }

        // Recursively draw child nodes
        for (TreeNode* childNode : node->m_children)
        {
            if (m_reset)
                break;

            DrawNodeAndChildren(childNode);
        }
        ::ui::EndTreeNode();
    }
}

const ::ui::Payload editor::SceneGraphUI::DragDropNode(const char* payloadID, TreeNode* node, int32 flags)
{
    if (::ui::StartDragDropTarget())
    {
        if (::ui::Payload payload = ::ui::AcceptPayload(payloadID, flags))
        {
            TreeNode* selectedNode = reinterpret_cast<TreeNode*>(payload.GetData());

            if (payload.IsDelivery() && node->m_handle != selectedNode->m_parent->m_handle)
            {
                bool blockAction = false;

                TreeNode* prevParent = node->m_parent;
                while (prevParent->m_handle != engine::Entity::EHandleUtils::INVALID_HANDLE)
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
        }

        ::ui::EndDragDropTarget();
    }

    return nullptr;
}

const ::ui::Payload editor::SceneGraphUI::DragDropBackground(const char* payloadID, int32 flags)
{
    if (::ui::StartDragDropTargetCustom(this->GetWindowRect(), m_title.c_str()))
    {
        if (::ui::Payload payload = ::ui::AcceptPayload(payloadID, flags))
        {
            if (payload.IsPreview())
            {
                ::ui::DrawList drawList = ::ui::GetForegroundDrawList();
                auto transform = this->GetWindowRect();
                uint32 color = ::ui::GetColor(::ui::DRAG_DROP_TARGET_COLOR, 0.05f);
                drawList.AddRectFilled(transform.m_min, transform.m_max, color);
            }

            TreeNode* selectedNode = reinterpret_cast<TreeNode*>(payload.GetData());
            if (payload.IsDelivery() && selectedNode->m_parent->m_handle != engine::Entity::EHandleUtils::INVALID_HANDLE)
            {
                ReparentNode(selectedNode, m_root);

                return payload;
            }
        }
        ::ui::EndDragDropTarget();
    }

    return nullptr;
}

int32 editor::SceneGraphUI::GetNodeFlags(TreeNode* node) const
{
    // Default flags
    int32 nodeFlags = ::ui::SPAN_FULL_WIDTH | ::ui::FRAME_PADDING | ::ui::OPEN_ON_DOUBLE_CLICK | ::ui::OPEN_WITH_ARROW;

    // Only add 'open / close' icon if node contains children
    if (node->m_children.empty())
        nodeFlags |= ::ui::NO_ICON;

    if (m_selectedHandle == node->m_handle)
        nodeFlags |= ui::SELECTED;

    return nodeFlags;
}

int32 editor::SceneGraphUI::GetDragDropFlags(int64 const nodeHandle) const
{
    /*
        Drag Drop flags dependent on if node currently being renamed
        (due to renaming removing the ID).
    */
    return
        (nodeHandle == m_renamingHandle) ? ::ui::EDragDropSrcFlags::ALLOW_NULL_ID : 0;
}

void editor::SceneGraphUI::RenderMenuBar(void)
{
    ::ui::StartDisabledSection(!m_graph);
    if (::ui::StartMenuBar())
    {
        if (::ui::Button("Add Entity"))
            AddNode(m_root);

        ::ui::EndMenuBar();
    }
    ::ui::EndDisabledSection();
}

void editor::SceneGraphUI::RenderPopupMenu(TreeNode* node)
{
    // Scene graph right click menu
    if (::ui::StartPopUp("Options"))
    {
        // Add
        if (::ui::MenuItem("Add Entity"))
            AddNode(node);

        // Rename
        if (::ui::MenuItem("Rename"))
            m_renamingHandle = node->m_handle;

        // Delete
        if (::ui::MenuItem("Delete"))
        {
            m_graph->DestroyEntity(node->m_handle);
            TreeNode* parentNode = node->m_parent;
            for (TreeNode* childNode : node->m_children)
                ReparentNode(childNode, parentNode);

            DeleteAllChildren(node);
        }

        ::ui::EndPopUp();
    }
}
