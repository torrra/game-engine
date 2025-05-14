#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include <math/Vector2.hpp>
#include <vector>

namespace ui
{
    // Components
    ENGINE_API void Text(const char* text, ...);
    ENGINE_API void Image(uint64 textureID, math::Vector2f const& size);
    ENGINE_API bool Button(const char* text, math::Vector2f const& size = {0.0F, 0.0F});
    ENGINE_API bool DropDown(const char* id, int32& selectedValue, std::vector<const char*> const& options);
    ENGINE_API bool InputBox(const char* id, const char* hint, std::string& outStr, bool enterReturnTrue = true);
    ENGINE_API bool InputBox(const char* id, f32* value, f32 increment, bool returnOnComplete = false);
    ENGINE_API bool Selectable(const char* text, bool* selected, math::Vector2f const& size);
    ENGINE_API bool Checkbox(const char* text, bool* value);

    // Menus
    ENGINE_API bool StartMenuBar(void);
    ENGINE_API bool StartMainMenuBar(void);
    ENGINE_API bool StartMenu(const char* name);
    ENGINE_API void EndMenuBar(void);
    ENGINE_API void EndMainMenuBar(void);
    ENGINE_API void EndMenu(void);
    ENGINE_API bool MenuItem(const char* name, const char* shortcut = nullptr);

    // PopUps
    ENGINE_API bool StartModal(const char* name, bool& shouldClose);
    ENGINE_API bool StartPopUp(const char* name);
    ENGINE_API void EndPopUp(void);
    ENGINE_API void OpenModal(const char* name);

    // Layout
    ENGINE_API void SameLine(f32 xOffset = -1.0f);
    ENGINE_API void ItemWidth(f32 widthPx);
    ENGINE_API void VerticalSpacing(void);
    ENGINE_API math::Vector2f GetPos(void);
    ENGINE_API math::Vector2f GetScreenPos(void);
    ENGINE_API math::Vector2f GetAvailSpace(void);
    ENGINE_API math::Vector2i GetViewportPos(std::string const& windowName);
    ENGINE_API math::Vector2i GetWindowPos(std::string const& windowName);
    ENGINE_API math::Vector2f GetWindowPos(void);
    ENGINE_API math::Vector2i GetOuterRectMinPos(std::string const& windowName);
    ENGINE_API math::Vector2i GetInnerRectMinPos(std::string const& windowName);
    ENGINE_API void SetAlignment(math::Vector2f const& gridIndex);
    ENGINE_API void SetScreenPosition(math::Vector2f const& position);
    ENGINE_API void UnsetAlignment(void);

    // Sections
    ENGINE_API bool StartSection(const char* name);
    ENGINE_API bool StartSection(const char* name, math::Vector2f const& size);
    ENGINE_API void EndSection(void);
    ENGINE_API void StartDisabledSection(bool disabled = true);
    ENGINE_API void EndDisabledSection(void);
    ENGINE_API bool CollapseSection(const char* name, bool& closeButton);

    // IDs
    ENGINE_API void SetID(std::string const& id);
    ENGINE_API void SetID(int32 id);
    ENGINE_API void UnsetID(void);

    // Misc...
    ENGINE_API void SetKeyboardFocus(void);
    ENGINE_API bool IsItemSelected(void);
    ENGINE_API bool IsItemHovered(void);
    ENGINE_API bool IsWindowSelected(std::string const& name);
    ENGINE_API bool IsWindowDocked(std::string const& windowName);
    ENGINE_API bool IsRectVisible(math::Vector2f const& size);
    ENGINE_API bool IsRectVisible(math::Vector2f const& min, math::Vector2f const& max);
}