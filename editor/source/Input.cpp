#include "Input.h"
#include <engine/input/InputHandler.h>
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIManager.h>

void editor::LockMousePosition(const char* wndName, bool& state)
{
    if (engine::InputHandler::IsInputPressed(KEY_ESCAPE))
    {
        state = !state;

        engine::InputHandler::SetCursorMode(
            (state) ?
            engine::ECursorMode::MODE_DISABLED : engine::ECursorMode::MODE_NORMAL
        );
    }

    if (state)
        engine::UIManager::LockCursor();
}
