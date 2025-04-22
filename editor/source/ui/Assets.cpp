#include "ui/Assets.h"
#include "ui/components/Component.h"
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDraw.h>
#include <engine/ui/UITree.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/input/Input.h>
#include <filesystem>

#define ASSET_WIDTH 120.0f
#define ASSET_HEIGHT 160.0f
#define ASSET_PADDING 15.0f

// Node implementation
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

// Asset implementation
editor::Asset::Asset(std::filesystem::path const& path, f32 width, f32 padding)
    : m_path(path), m_width(width), m_padding(padding)
{}

// Asset window implementation
editor::AssetsWnd::AssetsWnd(const char* name)
{
    SetName(name);

    auto workingDir = std::filesystem::current_path();
    m_rootNode = InitDirectoryRecursively(workingDir);
    std::cout << "Working directory: " << workingDir << '\n';

    m_layout = new ui::Table("AssetWnd", 2);
    m_layout->SetFlags(ui::ETableFlags::TABLE_INNER_VERTICAL_BORDERS | ui::ETableFlags::TABLE_RESIZABLE);

    m_assets = new ui::Table("AssetSection", 7);

    m_selectedNode = nullptr;
}

editor::AssetsWnd::~AssetsWnd(void)
{
    if (m_rootNode)
        delete m_rootNode;

    delete m_layout;
    delete m_assets;

    m_layout = nullptr;
    m_assets = nullptr;
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
        {
            RenderAssets();
        }
    
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

void editor::AssetsWnd::RenderAssets(void)
{
    //ui::DrawList drawList = ui::GetWindowDrawList();
    //drawList.AddRectFilled({1000.0f, 1000.0f}, ui::GetAvailSpace(), {0.0f, 1.0f, 0.0f, 1.0f}, 25.0f);
    //m_assets->StartTable();
    //uint16 index = 0;
    //for (auto file : m_files)
    //{
    //    if (index == m_assets->GetMaxColumns())
    //    {
    //        m_assets->NextRow();
    //        index = 0;
    //    }
    //
    //    m_assets->NextColumn();
    //    RenderFile(file);
    //
    //    ++index;
    //}
    //m_assets->EndTable();

    if (ui::StartSection("AssetsSection", ui::GetAvailSpace()))
    {
        ui::DrawList drawList = ui::GetWindowDrawList();
        
        // Content region
        math::Vector2f min = drawList.GetMin(); min;
        math::Vector2f max = drawList.GetMax(); max;
        uint16 itemPerRow = static_cast<uint16>((max - min).GetX() / (ASSET_WIDTH + (2.0f * ASSET_PADDING)));

        math::Vector2f position = min + math::Vector2f(ASSET_PADDING, ASSET_PADDING);

        uint16 index = 0;
        math::Vector2<uint16> gridIndex(0);
        for (const Asset& asset : m_files)
        {
            ui::SetID(asset.m_path.string());
            if (gridIndex.GetX() == itemPerRow)
            {
                // Next row
                ++gridIndex.Y();
                gridIndex.X() = 0;

                position.Y() = (gridIndex.GetY() * ASSET_PADDING) + ((gridIndex.GetY() - 1) * ASSET_HEIGHT);

            }
           
            position.X() = (gridIndex.GetX() * ASSET_PADDING) + ((gridIndex.GetX() - 1) * ASSET_WIDTH);

            uint16 i = ((gridIndex.GetY()) * itemPerRow) + gridIndex.GetX();

            bool selected = false;
            if (gridIndex.GetX() != 0)
                ui::SameLine(position.GetX());
            
            ui::SetAlignment(position);
            bool isItemVisible = ui::IsRectVisible({ASSET_WIDTH, ASSET_HEIGHT});
            ui::Selectable(m_files[i].m_path.filename().string().c_str(), &selected, {ASSET_WIDTH, ASSET_HEIGHT});
            ui::UnsetAlignment();
            
            if (isItemVisible)
                drawList.AddRectFilled(min + position, min + position + math::Vector2f(ASSET_WIDTH, ASSET_HEIGHT), {0.1f, 0.1f, 0.1f, 1.0f});

            ++gridIndex.X();
            ui::UnsetID();
        }
        
        ui::EndSection();

    }
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
            m_files.emplace_back(Asset(filePath, ASSET_WIDTH, ASSET_PADDING));
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

void editor::AssetsWnd::RenderFile(std::filesystem::path const& file)
{

    //drawList.AddRectFilled()
    ui::Text("%s\n", file.filename().string().c_str());
}


