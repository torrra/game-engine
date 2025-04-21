#include "ui/Assets.h"
#include "ui/components/Component.h"
#include <engine/ui/UIComponent.h>
#include <engine/ui/UITree.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/input/Input.h>
#include <filesystem>

editor::DirTreeNode::DirTreeNode(std::filesystem::path const& path, DirTreeNode* parent)
    : m_path(path), m_parentNode(parent)
{
    m_dirName = path.filename().string();
}

editor::DirTreeNode::~DirTreeNode(void)
{
    for (DirTreeNode* dirNode : m_children)
        delete dirNode;

    m_children.clear();
    m_parentNode = nullptr;
}

editor::AssetsWnd::AssetsWnd(const char* name)
{
    SetName(name);

    auto workingDir = std::filesystem::current_path();
    m_rootNode = InitDirectoryRecursively(workingDir);
    std::cout << "Working directory: " << workingDir << '\n';

    m_layout = new ui::Table("AssetWnd", 2);
    m_layout->SetFlags(ui::ETableFlags::TABLE_INNER_VERTICAL_BORDERS | ui::ETableFlags::TABLE_RESIZABLE);

    m_selectedNode = nullptr;
    m_newNodeSelected = false;

    //m_rootNode = new DirTreeNode(workingDir, workingDir.string(), nullptr);
    //for (const auto& entry : std::filesystem::recursive_directory_iterator(workingDir))
    //{
    //    entry;
    //    std::cout << entry.path().extension() << std::endl;
    //}
}

editor::AssetsWnd::~AssetsWnd(void)
{
    if (m_rootNode)
        delete m_rootNode;

    delete m_layout;

    m_layout = nullptr;
    m_selectedNode = nullptr;
    m_rootNode = nullptr;
}

void editor::AssetsWnd::RenderContents(void)
{
    math::Vector2f windowSize = ::ui::GetAvailSpace();
    m_layout->SetSize(windowSize);

    if (m_layout->StartTable())
    {
        if (m_layout->NextColumn(150.0f))
            RenderDirectorySection(windowSize);

        if (m_selectedNode && m_layout->NextColumn())
            ui::Text("%s", m_selectedNode->m_dirName.c_str());

        m_layout->EndTable();
    }
}

void editor::AssetsWnd::RenderDirectorySection(math::Vector2f const& windowSize)
{
    RenderDirectories(m_rootNode);
}

void editor::AssetsWnd::RenderDirectories(DirTreeNode* node)
{
    bool isOpen = ::ui::TreeNode(node->m_dirName, GetTreeNodeFlags(node));

    // Select node
    if (ui::IsItemSelected() && (!m_selectedNode || m_selectedNode->m_dirName != node->m_dirName))
    {
        m_selectedNode = node;
        OnSelectDir();
    }

    if (isOpen)
    {
        for (DirTreeNode* childNode : node->m_children)
        {
            RenderDirectories(childNode);
        }

        ::ui::EndTreeNode();
    }
}

editor::DirTreeNode* editor::AssetsWnd::InitDirectoryRecursively(std::filesystem::path const& path, DirTreeNode* parentNode)
{
    DirTreeNode* directory = new DirTreeNode(path, parentNode);

    // Iterate through all entries in the current directory
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        // Ignore non directory entries
        if (!entry.is_directory())
            continue;

        directory->m_children.emplace_back(InitDirectoryRecursively(entry.path(), directory));
    }

    return directory;
}

int32 editor::AssetsWnd::GetTreeNodeFlags(DirTreeNode* node)
{
    int32 result = ui::ETreeNodeFlags::SPAN_FULL_WIDTH | ui::ETreeNodeFlags::OPEN_ON_DOUBLE_CLICK | ui::ETreeNodeFlags::OPEN_WITH_ARROW;

    if (node->m_children.empty())
        result |= ui::ETreeNodeFlags::NO_ICON;
    if (m_selectedNode && m_selectedNode->m_dirName == node->m_dirName)
        result |= ui::ETreeNodeFlags::SELECTED;

    return result;
}

void editor::AssetsWnd::OnSelectDir(void)
{
    // Remove all files
    m_files.clear();

    // Append file to array
    for (auto file : std::filesystem::directory_iterator(m_selectedNode->m_path))
    {
        const std::filesystem::path& filePath = file.path();

        if (IsSupportedExtension(filePath.extension().string()))
            m_files.emplace_back(filePath);
        //printf("%s | %s\n", file.path().filename().string().c_str(), file.path().extension().string().c_str());
    }
}

bool editor::AssetsWnd::IsSupportedExtension(std::string const& extension)
{
    constexpr const char* supportedExtensions[] =
        {".obj", ".fbx", ".dae", ".png", ".ttf", ".lua", ".vert", ".frag"};

    // Skip loop if entry is a folder
    if (extension[0] != '.')
        return false;

    // Check file has a valid extension
    for (const char* supportedExtension : supportedExtensions)
    {
        if (extension == supportedExtension)
            return true;
    }

    // Entry does not have a supported extension
    return false;
}

