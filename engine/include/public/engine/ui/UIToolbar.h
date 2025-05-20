#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include <string>
#include <vector>

namespace ui
{
    enum EToolbarOrientation
    {
        TOOLBAR_NONE = -1,
        TOOLBAR_HORIZONTAL = 0,
        TOOLBAR_VERTICAL = 1
    };

    struct ToolbarButton
    {
        ToolbarButton(const char* displayLabel);
        std::string m_displayLabel;
        bool m_value;
    };

    class Toolbar
    {
    public:
        ENGINE_API Toolbar(void) = delete;
        ENGINE_API Toolbar(const char* name);
        ENGINE_API ~Toolbar(void) = default;

        ENGINE_API uint32 AddButton(const char* displayLabel);
        ENGINE_API bool GetButtonState(uint32 index);

        
        
        ENGINE_API void Render(void);
    private:
        std::string m_name;
        std::vector<ToolbarButton> m_buttons;
        EToolbarOrientation m_orientation;
    };
}