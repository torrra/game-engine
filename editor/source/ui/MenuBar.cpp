#include "ui/MenuBar.h"
#include <engine/Engine.h>
#include <engine/ui/UIComponent.h>
#include <engine/game/GameScene.h>

#include "ui/SceneGraph.h"

void editor::MenuBar::Render(::engine::Engine& engine)
{
    (void)engine;
}

void editor::MenuBar::UpdateStartButton(engine::GameScene& activeScene, editor::SceneGraphUI& graphWindow)
{
    ::ui::StartMainMenuBar();

    if (m_gameRunning && ::ui::Button("Stop"))
    {
        m_gameRunning = false;
        activeScene.Stop();
        printf("Game running: %s\n", "false");

        graphWindow.ClearGraph();
        activeScene.Reset();
        graphWindow.SetGraph(activeScene.GetGraph());
    }
    else if (!m_gameRunning && ::ui::Button("Start"))
    {
        activeScene.Start();
        m_gameRunning = true;
        printf("Game running: %s\n", "true");
    }

    ::ui::EndMainMenuBar();
}
