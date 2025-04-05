#pragma once

#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>

#undef new
#include <imgui/imgui.h>

struct ImRect;

namespace engine::ui
{
    enum EWndFlags
    {   
        NONE                            = 0,
        NO_TITLE_BAR                    = ImGuiWindowFlags_NoTitleBar,
        NO_RESIZE                       = ImGuiWindowFlags_NoResize,
        NO_MOVE                         = ImGuiWindowFlags_NoMove,
        NO_SCROLL_BAR                   = ImGuiWindowFlags_NoScrollbar,
        NO_MOUSE_SCROLL                 = ImGuiWindowFlags_NoScrollWithMouse,
        NO_MOUSE_INPUT                  = ImGuiWindowFlags_NoMouseInputs,
        NO_COLLAPSE                     = ImGuiWindowFlags_NoCollapse,
        AUTO_RESIZE                     = ImGuiWindowFlags_AlwaysAutoResize,
        NO_BACKGROUND                   = ImGuiWindowFlags_NoBackground,
        NO_META_DATA                    = ImGuiWindowFlags_NoSavedSettings,
        MENU_BAR                        = ImGuiWindowFlags_MenuBar,
        HORIZONTAL_SCROLL_BAR           = ImGuiWindowFlags_HorizontalScrollbar,
        NO_FOCUS_ON_APPEARING           = ImGuiWindowFlags_NoFocusOnAppearing,
        NO_BRING_TO_FRONT_ON_FOCUS      = ImGuiWindowFlags_NoBringToFrontOnFocus,
        ALWAYS_VERTICAL_SCROLL_BAR      = ImGuiWindowFlags_AlwaysVerticalScrollbar,
        ALWAYS_HORIZONTAL_SCROLL_BAR    = ImGuiWindowFlags_AlwaysHorizontalScrollbar,
        NO_NAV_INPUTS                   = ImGuiWindowFlags_NoNavInputs,
        NO_NAV_FOCUS                    = ImGuiWindowFlags_NoNavFocus,
        NO_DOCKING                      = ImGuiWindowFlags_NoDocking,
        NO_NAV                          = ImGuiWindowFlags_NoNav,
        NO_DECORATIONS                  = ImGuiWindowFlags_NoDecoration,
        NO_INPUTS                       = ImGuiWindowFlags_NoInputs
    };

    struct UIWindowRect
    {
        UIWindowRect(void) = default;
        UIWindowRect(math::Vector2f min, math::Vector2f max);

        operator ImRect(void);
        operator ImRect(void) const;

        math::Vector2f m_min{0.0f, 0.0f};
        math::Vector2f m_max{0.0f, 0.0f};
    };

    void StartWindowRenderer(const char* title, int32 flags);
    void EndWindowRenderer(void);
    
    void SetWindowPos(math::Vector2f position, int32 flag);
    void SetWindowSize(math::Vector2f size, int32 flag);
    const UIWindowRect GetCurrentWindowRect(void);
}