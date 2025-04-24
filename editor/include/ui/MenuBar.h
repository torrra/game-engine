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
        MenuBar(void) = default;

        void Render(:engine::Engine& engine);
        void UpdateStartButton(::engine::GameScene& scene, class SceneGraphUI& graph);

    private:
        bool m_gameRunning = false;
    };
}