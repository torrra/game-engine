#include "ui/MenuBar.h"

#include <engine/Engine.h>
#include <engine/ui/UIComponent.h>
#include <engine/game/GameScene.h>

#include "ui/SceneGraph.h"

void editor::MenuBar::Render(::engine::Engine& engine)
{
    ::ui::StartMainMenuBar();

    static bool gameState = false;

    // Start / stop game runtime
    if (::ui::Button(gameState ? "Stop" : "Start"))
    {
        gameState = !gameState;
        engine.SetGameState(gameState);
        printf("Game running: %s\n", gameState ? "true" : "false");
    }

    ::ui::EndMainMenuBar();
}

void editor::MenuBar::UpdateStartButton(engine::GameScene& activeScene, editor::SceneGraphUI& graphWindow)
{
    ::ui::StartMainMenuBar();

    static bool gameState = false;

    if (gameState && ::ui::Button("Stop"))
    {
        gameState = false;
        activeScene.Stop();
        printf("Game running: %s\n", "false");

        graphWindow.ClearGraph();
        activeScene.Reset();
        graphWindow.SetGraph(activeScene.GetGraph());
    }
    else if (!gameState && ::ui::Button("Start"))
    {
        activeScene.Start();
        gameState = true;
        printf("Game running: %s\n", "true");
    }

    ::ui::EndMainMenuBar();
}
