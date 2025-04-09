#include "ui/MenuBar.h"

#include <engine/Engine.h>
#include <engine/ui/UIComponent.h>

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
