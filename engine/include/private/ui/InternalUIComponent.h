#pragma once

#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>

namespace engine::ui
{
    // Components
    void Text(const char* text, ...);
    bool Button(const char* text, math::Vector2f size = {0.0F, 0.0F});
    bool InputTextBox(const char* hint, std::string& outStr);

    // Menus
    bool StartMenuBar(void);
    void EndMenuBar(void);
    bool MenuItem(const char* name, const char* shortcut = nullptr);

    // PopUps
    bool StartPopUp(const char* name);
    void EndPopUp(void);

    // Layout
    void SameLine(void);

    // Sections
    void StartDisabledSection(bool disabled = true);
    void EndDisabledSection(void);

    // IDs
    void SetID(std::string const& id);
    void UnsetID(void);

    // Misc...
    void SetKeyboardFocus(void);
}