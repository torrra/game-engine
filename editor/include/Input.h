#pragma once

namespace editor
{
    // Takes the window to lock as a parameter & cursor lock state
    void LockMousePosition(const char* wndName, bool& state);
}