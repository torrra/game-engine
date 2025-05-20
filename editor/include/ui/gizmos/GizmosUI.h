#pragma once

#include <engine/ui/UIToolbar.h>
#include <engine/CoreTypes.h>

namespace editor
{
    class GizmosUI
    {
    public:
        GizmosUI(void);
        ~GizmosUI(void);

        void Render(void);

    private:
        ui::Toolbar* m_toolbar;
        uint32 m_translateButton;
        uint32 m_rotateButton;
        uint32 m_scaleButton;
    };
}