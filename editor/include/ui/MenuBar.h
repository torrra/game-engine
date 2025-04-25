#pragma once

namespace engine
{
    class Engine;
    class GameScene;
}

namespace editor
{
    class MenuBar
    {
    public:
        MenuBar(class EditorApplication* application);

        void Render(::engine::Engine& engine);
        void UpdateStartButton(::engine::GameScene& scene);

    private:
        class EditorApplication* m_application = nullptr;
        bool m_gameRunning = false;
    };
}