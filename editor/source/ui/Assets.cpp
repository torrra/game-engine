#include "ui/Assets.h"
#include "ui/components/Component.h"
#include "ui/EditorApplication.h"

#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDraw.h>
#include <engine/ui/UITree.h>
#include <engine/ui/UIStyle.h>
#include <engine/ui/UIDragDrop.h>
#include <engine/ConsoleLog.hpp>
#include <engine/core/systems/ScriptSystem.h>

#include <engine/utility/MemoryCheck.h>
#include <engine/utility/Platform.h>
#include <engine/input/InputHandler.h>
#include <engine/Engine.h>
#include <filesystem>
#include <fstream>

#define ASSET_WIDTH 120.0f
#define ASSET_HEIGHT 130.0f
#define ASSET_PADDING 15.0f
#define MAX_LABEL_LINE_LENGTH 16
#define SUPPORTED_EXTENSIONS {".obj", ".fbx", ".dae", ".png", ".ttf", ".lua", ".vert", ".frag", ".mmat", ".mscn", ".ogg", ".mp3"}


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool DirUpdated(std::string const& path)
{
    HANDLE handles[1] = { nullptr };

    handles[0] = FindFirstChangeNotificationA(path.c_str(), false, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);
    if (handles[0] == INVALID_HANDLE_VALUE || handles[0] == nullptr)
    {
        printf("[ERROR]: FindFirstChangeNotificationA failed, error code: %lu.\n", GetLastError());
        return false;
    }

    DWORD result = WaitForMultipleObjects(1, handles, false, 500);
    bool newChanges = false;

    switch (result)
    {
    case WAIT_OBJECT_0:
        // Refresh directory
        printf("Refresh required\n");
        newChanges = true;

        if (!FindNextChangeNotification(handles[0]))
            printf("[ERROR]: FindNextChangeNotification failed, error code %lu.\n", GetLastError());

        break;
    case WAIT_TIMEOUT:
        break;
    default:
        printf("[ERROR]: An unhandled case occurred while checking for project directory changes\n");
        break;
    }

    return newChanges;
}


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
editor::AssetsWnd::AssetsWnd(const char* name, EditorApplication* owner)
    : m_ownerApplication(owner)
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

void editor::AssetsWnd::SetPath(std::filesystem::path const& projectDir)
{
    m_selectedIndex = -1;
    m_selectedDirectory = nullptr;
    
    if (m_rootNode)
        delete m_rootNode;

    m_assets.clear();

    m_path = projectDir;
    m_rootNode = InitDirectoryRecursively(m_path);
}

void editor::AssetsWnd::RenderContents(void)
{
    bool shouldRefresh = false;

    if (m_isDirUpdated.valid() && m_isDirUpdated.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
    {
        if (m_isDirUpdated.get())
            shouldRefresh = true;

        else if (m_selectedDirectory)
            m_isDirUpdated = engine::ThreadManager::AddTaskWithResult(&DirUpdated, m_selectedDirectory->m_path.string());
    }

    math::Vector2f windowSize = ::ui::GetAvailSpace();

    if (windowSize.GetX() <= 0.f || windowSize.GetY() <= 0.f)
        return;

    m_layout->SetSize(windowSize);

    if (m_layout->StartTable())
    {
        if (m_layout->NextColumn(150.0f))
            RenderDirectorySection(windowSize);
    
        if (m_layout->NextColumn())
        {
            RenderAssets();
            EAssetAction rightClickMenu = RenderRightClickMenu();

            if (m_currentAction == EAssetAction::NONE)
                m_currentAction = rightClickMenu;

            switch (m_currentAction)
            {
            
            case editor::AssetsWnd::EAssetAction::CREATE_SCENE:
            case editor::AssetsWnd::EAssetAction::CREATE_MATERIAL:
            case editor::AssetsWnd::EAssetAction::CREATE_SCRIPT:

                ui::OpenModal("New asset");

                if (CreateAsset(m_currentAction) == EAssetAction::REFRESH_WINDOW)
                    shouldRefresh = true;
                break;

            default:
                break;
            }
        }
    
        m_layout->EndTable();
    }

    if (shouldRefresh)
        OnSelectDir();
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

        ui::SetWindowFontScale(0.8f);

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
                    {
                        m_selectedIndex = index;
                        SelectResource();
                    }

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
            std::string relativeFilePath = filePath.string().substr(offset);

            m_assets.emplace_back(Asset(relativeFilePath, payloadType));
        }
    }

    if (m_isDirUpdated.valid())
        m_isDirUpdated.get();

    m_isDirUpdated = engine::ThreadManager::AddTaskWithResult(&DirUpdated, m_selectedDirectory->m_path.string());

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
    else if (extension == supportedExtensions[8])
        payloadType = MATERIAL_PAYLOAD;
    else if (extension == supportedExtensions[9])
        payloadType = SCENE_PAYLOAD;
    else if (extension == supportedExtensions[10])
        payloadType = AUDIO_PAYLOAD;
    else if (extension == supportedExtensions[11])
        payloadType = AUDIO_PAYLOAD;

    return payloadType;
}

void editor::AssetsWnd::SelectResource(void)
{
    constexpr uint64 sceneStrLen = sizeof(SCENE_PAYLOAD) - 1;
    constexpr uint64 scriptStrLen = sizeof(SCRIPT_PAYLOAD) - 1;

    Asset& selectedAsset = m_assets[m_selectedIndex];

    if (selectedAsset.m_payloadType.size() == sceneStrLen &&
        memcmp(selectedAsset.m_payloadType.c_str(), SCENE_PAYLOAD, sceneStrLen) == 0)
    {
        std::filesystem::path scenePath = engine::Engine::GetEngine()->GetProjectDir();

        scenePath += selectedAsset.m_path;
        m_ownerApplication->LoadNewScene(engine::Engine::GetEngine()->GetCurrentScene(),
            scenePath);
    }

    else if (selectedAsset.m_payloadType.size() == scriptStrLen &&
        memcmp(selectedAsset.m_payloadType.c_str(), SCRIPT_PAYLOAD, scriptStrLen) == 0)
    {
        std::filesystem::path scriptPath = engine::Engine::GetEngine()->GetProjectDir();

        scriptPath += selectedAsset.m_path;

        engine::OpenFile(scriptPath.c_str());
    }
}



editor::AssetsWnd::EAssetAction editor::AssetsWnd::RenderRightClickMenu(void)
{

    EAssetAction result = EAssetAction::NONE;

    // Scene graph right click menu
    if (::ui::StartPopUp("Asset options"))
    {
        // Add scene
        if (::ui::MenuItem("Create scene"))
            result = EAssetAction::CREATE_SCENE;

        //// Add material
        //if (::ui::MenuItem("Create material"))
        //    result = EAssetAction::CREATE_MATERIAL;

        //// Delete
        //if (::ui::MenuItem("Delete asset"))
        //    result = EAssetAction::DELETE_ASSET;

        if (::ui::MenuItem("Create script"))
            result = EAssetAction::CREATE_SCRIPT;

        ::ui::EndPopUp();
        m_isRightClickMenuOpen = true;
    }
    return result;
}

void editor::AssetsWnd::SetNewAssetName(void)
{
    ui::VerticalSpacing();

    // Display text + format layout
    m_assetCreationTable.NextRow();
    m_assetCreationTable.NextColumn();
    ui::Text("Asset name: ");
    ui::VerticalSpacing();

    // Add interactable input field for user to write the project name + format layout 
    m_assetCreationTable.NextColumn(500.0f);
    ui::ItemWidth(150.0f);
    ui::InputBox("##AssetName", "Asset name", m_newAssetName, false);
    ui::VerticalSpacing();

    //// Display current name
    m_assetCreationTable.NextColumn();
    ui::Text(m_newAssetName.c_str());
    ui::VerticalSpacing();
}

bool editor::AssetsWnd::IsAssetNameValid(void)
{
    return (!m_newAssetName.empty()) && m_newAssetName.size() < 64;
}

void editor::AssetsWnd::CreateScene(void)
{
    std::filesystem::path assetPath = m_selectedDirectory->m_path;

    assetPath.append(m_newAssetName).replace_extension(".mscn");

    if (std::filesystem::exists(assetPath))
        engine::PrintLog(engine::ErrorPreset(), "Scene file already exists");

    else
        std::ofstream newScene(assetPath, std::ios::out);

}

void editor::AssetsWnd::CloseAssetCreationMenu(void)
{
    m_isRightClickMenuOpen = false;
    m_newAssetName.clear();
    m_currentAction = EAssetAction::NONE;
}

editor::AssetsWnd::EAssetAction editor::AssetsWnd::SelectNewAssetType(EAssetAction action)
{
    EAssetAction result = EAssetAction::NONE;

    switch (action)
    {
    case editor::AssetsWnd::EAssetAction::CREATE_SCENE:
        CreateScene();
        result = EAssetAction::REFRESH_WINDOW;
        break;
    case editor::AssetsWnd::EAssetAction::CREATE_MATERIAL:
        break;

    case editor::AssetsWnd::EAssetAction::CREATE_SCRIPT:

        engine::ScriptSystem::CreateUserScript(
            nullptr,
            m_newAssetName.c_str()
        );
        result = EAssetAction::REFRESH_WINDOW;
        break;
    default:
        break;
    }

    return result;
}

editor::AssetsWnd::EAssetAction editor::AssetsWnd::CreateAsset(EAssetAction action)
{
    EAssetAction result = EAssetAction::NONE;

    if (ui::StartModal("New asset", m_isRightClickMenuOpen))
    {
        m_assetCreationTable.SetSize(ui::GetAvailSpace());

        if (engine::InputHandler::IsInputPressed(KEY_ESCAPE))
            CloseAssetCreationMenu();            

        if (m_assetCreationTable.StartTable())
        {
            ui::SameLine(160.f);
            SetNewAssetName();
            ui::VerticalSpacing();

            if (ui::Button("Create asset") && IsAssetNameValid())
            {
                result = SelectNewAssetType(action);
                CloseAssetCreationMenu();            
            }
            m_assetCreationTable.EndTable();
        }
        else
        {
            ui::SameLine(160.f);
            ui::Text("Failed");
        }

        ui::EndPopUp();
    }

    if (!m_isRightClickMenuOpen)
        m_currentAction = EAssetAction::NONE;

    return result;
}
