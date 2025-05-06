#pragma once

#include <filesystem>

namespace engine
{
    class GameScene;
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
        void ProjectMenu(void);

        void OpenProject(void);
        bool SelectProject(std::filesystem::path& projectPath);

        class EditorApplication* m_application = nullptr;
        bool m_gameRunning = false;
    };
}