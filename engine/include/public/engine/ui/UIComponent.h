#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>

namespace ui
{
    // Components
    ENGINE_API void Text(const char* text, ...);
    ENGINE_API bool Button(const char* text, math::Vector2f size = {0.0F, 0.0F});
    ENGINE_API bool InputBox(const char* id, const char* hint, std::string& outStr);
    ENGINE_API bool InputBox(const char* id, f32* value, f32 increment, bool returnOnComplete = false);

    // Menus
    ENGINE_API bool StartMenuBar(void);
    ENGINE_API bool StartMainMenuBar(void);
    ENGINE_API void EndMenuBar(void);
    ENGINE_API void EndMainMenuBar(void);
    ENGINE_API bool MenuItem(const char* name, const char* shortcut = nullptr);

    // PopUps
    ENGINE_API bool StartPopUp(const char* name);
    ENGINE_API void EndPopUp(void);

    // Layout
    ENGINE_API void SameLine(void);
    ENGINE_API void ItemWidth(f32 widthPx);
    ENGINE_API void VerticalSpacing(void);

    // Sections
    ENGINE_API void StartSection(const char* name);
    ENGINE_API void EndSection(void);
    ENGINE_API void StartDisabledSection(bool disabled = true);
    ENGINE_API void EndDisabledSection(void);
    ENGINE_API bool CollapseSection(const char* name);

    // IDs
    ENGINE_API void SetID(std::string const& id);
    ENGINE_API void UnsetID(void);

    // Misc...
    ENGINE_API void SetKeyboardFocus(void);
    ENGINE_API bool IsItemSelected(void);
    ENGINE_API bool IsItemHovered(void);
}