#include "Input.h"
#include <engine/input/InputHandler.h>
#include <engine/ui/UIComponent.h>

void editor::LockMousePosition(const char* wndName, bool& state)
{
    // Update cursor mode
    if (ui::IsWindowSelected(wndName) && engine::InputHandler::IsInputPressed(KEY_ESCAPE))
    {
        state = !state;

        // Set cursor mode
        engine::InputHandler::SetCursorMode(
            (state) ?
            engine::ECursorMode::MODE_DISABLED : engine::ECursorMode::MODE_NORMAL
        );
    }
    // Condition prevents cursor becoming stuck in a locked cursor state if the selected window changes
    else if (!ui::IsWindowSelected(wndName) && engine::InputHandler::IsInputPressed(KEY_ESCAPE))
    {
        state = false;
        engine::InputHandler::SetCursorMode(engine::ECursorMode::MODE_NORMAL);
    }

}
