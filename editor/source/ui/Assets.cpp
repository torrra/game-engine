#include "ui/Assets.h"
#include "ui/components/Component.h"
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDraw.h>
#include <engine/ui/UITree.h>
#include <engine/ui/UIStyle.h>
#include <engine/ui/UIDragDrop.h>

#include <engine/utility/MemoryCheck.h>
#include <engine/input/Input.h>
#include <filesystem>

#define ASSET_WIDTH 120.0f
#define ASSET_HEIGHT 130.0f
#define ASSET_PADDING 15.0f
#define MAX_LABEL_LINE_LENGTH 16
#define SUPPORTED_EXTENSIONS {".obj", ".fbx", ".dae", ".png", ".ttf", ".lua", ".vert", ".frag", ".mmat", ".mscn", ".ogg", ".mp3"}

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
editor::Asset::Asset(std::filesystem::path const& path, std::string const& payloadType)
    : m_path(path), m_payloadType(payloadType)
{
    m_fileName = path.filename().string();
}

// Asset window implementation
editor::AssetsWnd::AssetsWnd(const char* name)
{
    SetName(name);

    m_path = std::filesystem::current_path();

    //auto workingDir = std::filesystem::current_path();
    m_rootNode = InitDirectoryRecursively(m_path);
    std::cout << "Working directory: " << m_path << '\n';

    m_layout = new ui::Table("AssetWnd", 2);
    m_layout->SetFlags(ui::ETableFlags::TABLE_INNER_VERTICAL_BORDERS | ui::ETableFlags::TABLE_RESIZABLE);

    m_selectedDirectory = nullptr;
    m_selectedIndex = -1;
}

editor::AssetsWnd::~AssetsWnd(void)
{
    if (m_rootNode)
        delete m_rootNode;

    delete m_layout;

    m_layout = nullptr;
    m_selectedDirectory = nullptr;
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
    
        if (m_layout->NextColumn())
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
    if (ui::IsItemSelected() && (!m_selectedDirectory || m_selectedDirectory->m_dirName != node->m_dirName))
    {
        m_selectedDirectory = node;
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
    if (m_selectedDirectory && m_selectedDirectory->m_dirName == node->m_dirName)
        result |= ui::ETreeNodeFlags::SELECTED;

    return result;
}

void editor::AssetsWnd::RenderAssets(void)
{
    const math::Vector2f& regionSize = ui::GetAvailSpace();

    if (ui::StartSection("AssetSection", regionSize))
    {
        // Asset variables
        static const math::Vector2f assetSize(ASSET_WIDTH, ASSET_HEIGHT);
        static const f32 sizeDenominator = 1.0f / (assetSize.GetX() + ASSET_PADDING);
        int32 index = 0;

        // Window variables
        const math::Vector2f& minPos = ui::GetWindowPos();
        const math::Vector2f& maxPos = minPos + regionSize;
        const math::Vector2f& startPos = ui::GetScreenPos() + math::Vector2f(ASSET_PADDING);
        ui::DrawList drawList = ui::GetWindowDrawList();
        int32 itemPerRow = math::Max((int32) (regionSize.GetX() * sizeDenominator), 1);
        int32 assetCount = static_cast<int32>(m_assets.size());

        ui::SetScreenPosition(startPos);

        m_clipper.Begin(assetCount, assetSize.GetY());
        
        while (m_clipper.Step())
        {
            // Iterate through all items
            for (int32 lineNumber = m_clipper.GetStart(); lineNumber < m_clipper.GetEnd(); ++lineNumber)
            {
                // Calculate which item id are in current row
                int32 minIdCurrentRow = lineNumber * itemPerRow;
                int32 maxIdCurrentRow = math::Min<int32>((lineNumber + 1) * itemPerRow, assetCount);

                math::Vector2f position(
                        0.0f,
                        startPos.GetY() + lineNumber * (assetSize.GetY() + ASSET_PADDING)
                );

                // Iterate through items in current row
                for (int32 item = minIdCurrentRow; item < maxIdCurrentRow; ++item)
                {
                    ui::SetID(index);

                    position.X() = startPos.GetX() + float(item % itemPerRow) * (assetSize.GetX() + ASSET_PADDING);
                    ui::SetScreenPosition(position);

                    bool isItemVisible = ui::IsRectVisible(minPos, maxPos);
                    bool isSelected = index == m_selectedIndex; // TODO: use index to get selected value from asset struct

                    if (ui::Selectable("", &isSelected, assetSize))
                        m_selectedIndex = index;

                    // Create drag & drop payload 
                    if (::ui::StartDragDropSource(0))
                    {
                        ::ui::CreatePayload(m_assets[index].m_payloadType.c_str(), &m_assets[index], sizeof(Asset));
                        ::ui::Text("%s", m_assets[index].m_fileName.c_str());
                        ::ui::EndDragDropSource();
                    }

                    if (isItemVisible)
                    {
                        math::Vector2f max = position + assetSize;
                        drawList.AddRectFilled(position, max, {0.1f, 0.1f, 0.1f, 1.0f}, 15.0f);
                        
                        // Label
                        std::string text = m_assets[index].m_fileName;

                        if (text.length() > MAX_LABEL_LINE_LENGTH)
                            text.insert(MAX_LABEL_LINE_LENGTH, "\n");

                        // Calculate text position
                        math::Vector2f textPosition(
                            position.GetX() + ((assetSize.GetX() - ui::GetTextSize(text.c_str()).GetX()) * 0.5f), // Center aligned
                            max.GetY() - (ASSET_PADDING + ui::GetFontSize())                                      // Bottom aligned
                        );

                        drawList.AddText(text.c_str(), textPosition, isSelected);
                    }
                    ui::UnsetID();
                    index++;
                }
            }
        }
        
        m_clipper.End();

        ui::EndSection();
    }
}

void editor::AssetsWnd::OnSelectDir(void)
{
    // Remove all files
    m_assets.clear();

    // Invalidate selected asset
    m_selectedIndex = -1;

    // Append file to array
    for (auto file : std::filesystem::directory_iterator(m_selectedDirectory->m_path))
    {
        const std::filesystem::path& filePath = file.path();
        std::string payloadType;

        if (IsSupportedExtension(filePath.extension().string(), payloadType))
        {
            uint64 offset = m_path.string().length();
            std::string relativeFilePath(".");
            relativeFilePath += filePath.string().substr(offset);

            m_assets.emplace_back(Asset(relativeFilePath, payloadType));
        }
    }
}

bool editor::AssetsWnd::IsSupportedExtension(std::string const& extension, std::string& payloadType)
{
    constexpr const char* supportedExtensions[] = SUPPORTED_EXTENSIONS;

    // Skip loop if entry is a folder
    if (extension[0] != '.')
        return false;

    // Check file has a valid extension
    for (const char* supportedExtension : supportedExtensions)
    {
        if (extension == supportedExtension)
        {
            payloadType = GetPayloadType(extension);
            return true;
        }
    }

    // Entry does not have a supported extension
    return false;
}

std::string editor::AssetsWnd::GetPayloadType(std::string const& extension) const
{
    constexpr const char* supportedExtensions[] = SUPPORTED_EXTENSIONS;
    std::string payloadType;

    if (extension == supportedExtensions[0] ||
        extension == supportedExtensions[1] ||
        extension == supportedExtensions[2])
        payloadType = MODEL_PAYLOAD;
    else if (extension == supportedExtensions[3])
        payloadType = TEXTURE_PAYLOAD;
    else if (extension == supportedExtensions[4])
        payloadType = FONT_PAYLOAD;
    else if (extension == supportedExtensions[5])
        payloadType = SCRIPT_PAYLOAD;
    else if (extension == supportedExtensions[6])
        payloadType = VERTEX_SHADER_PAYLOAD;
    else if (extension == supportedExtensions[7])
        payloadType = FRAGMENT_SHADER_PAYLOAD;

    return payloadType;
}

void editor::AssetsWnd::RenderFile(std::filesystem::path const& file)
{

    //drawList.AddRectFilled()
    ui::Text("%s\n", file.filename().string().c_str());
}


