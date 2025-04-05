#pragma once

#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#include <string>

namespace engine
{
    namespace ui
    {
        struct UIWindowRect;
    }

    enum EWndFlags
    {
        NONE = 0,
        NO_TITLE_BAR                    = 1 << 0,
        NO_RESIZE                       = 1 << 1,
        NO_MOVE                         = 1 << 2,
        NO_SCROLL_BAR                   = 1 << 3,
        NO_MOUSE_SCROLL                 = 1 << 4,
        NO_MOUSE_INPUT                  = 1 << 9,
        NO_COLLAPSE                     = 1 << 5,
        AUTO_RESIZE                     = 1 << 6,
        NO_BACKGROUND                   = 1 << 7,
        NO_META_DATA                    = 1 << 8,
        MENU_BAR                        = 1 << 10,
        HORIZONTAL_SCROLL_BAR           = 1 << 11,
        NO_FOCUS_ON_APPEARING           = 1 << 12,
        NO_BRING_TO_FRONT_ON_FOCUS      = 1 << 13,
        ALWAYS_VERTICAL_SCROLL_BAR      = 1 << 14,
        ALWAYS_HORIZONTAL_SCROLL_BAR    = 1 << 15,
        NO_NAV_INPUTS                   = 1 << 16,
        NO_NAV_FOCUS                    = 1 << 17,
        UNSAVED_DOC                     = 1 << 18,
        NO_DOCKING                      = 1 << 19,
        NO_NAV                          = NO_NAV_INPUTS | NO_NAV_FOCUS,
        NO_DECORATIONS                  = NO_TITLE_BAR | NO_RESIZE | NO_SCROLL_BAR | NO_COLLAPSE,
        NO_INPUTS                       = NO_MOUSE_INPUT | NO_NAV_INPUTS | NO_NAV_FOCUS
    };

    enum EActive
    {
        ALWAYS = 1,
        ONCE_PER_EXECUTION = 2,
        ONCE = 1 << 2,
        ON_APPEARING = 1 << 3
    };

    class UIWindow
    {
    public:
        UIWindow(void);
        ~UIWindow(void) = default;

        void Render(void);

    protected:
        virtual void RenderContents(void);
    
        ui::UIWindowRect GetWindowRect(void);

        void SetName(const char* title);
        void SetFlags(int32 flags);
        void SetWindowPos(math::Vector2f const& position, EActive activeFlag);
        void SetWindowSize(math::Vector2f const& size, EActive activeFlag);
        
        std::string m_title;
        int32 m_flags;
    };
}