#pragma once

#include <engine/CoreTypes.h>
#include <filesystem>

namespace engine
{
    class GameScene;
}

namespace ui
{
    class Table;
}

namespace editor
{
    class MenuBar
    {
    public:
        MenuBar(class EditorApplication* application);

        void Render(engine::GameScene& activeScene);

    private:
        void UpdateStartButton(::engine::GameScene& scene);
        void DisplayCurrentProject(void);
        void ProjectMenu(::engine::GameScene& scene);

        void OpenProject(::engine::GameScene& scene);
        bool SelectProject(std::filesystem::path& projectPath);

        void CreateProject(void);
        bool SelectFolder(std::filesystem::path& projectPath);
        void SetProjectName(ui::Table& contentTable, std::string& name);
        void SelectProjectPath(ui::Table& contentTable, std::filesystem::path& path);
        void CloseCreateMenu(std::string& projectName, std::filesystem::path& projectPath, uint8& status);

        class EditorApplication* m_application = nullptr;
        bool m_gameRunning = false;
        bool m_isCreateWndOpened = false;
    };
}