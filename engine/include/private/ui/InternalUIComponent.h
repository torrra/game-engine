#pragma once

#include <math/Vector2.hpp>

namespace engine::ui
{
    // Components
    void Text(const char* text, ...);
    bool Button(const char* text, math::Vector2f size = {0.0F, 0.0F});

    // Menus
    bool StartMenuBar(void);
    void EndMenuBar(void);

    // Sections
    void StartDisabledSection(bool disabled = true);
    void EndDisabledSection(void);
}