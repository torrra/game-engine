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

        void Render(class ::engine::Engine& engine);
        void UpdateStartButton(class ::engine::GameScene& scene, class SceneGraphUI& graph);
    };
}