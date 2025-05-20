#include "ui/gizmos/GizmosUI.h"
#include <engine/ui/IconsLucide.h>
#include <engine/utility/MemoryCheck.h>

editor::GizmosUI::GizmosUI(void)
{
    m_toolbar = new ui::Toolbar("GizmosToolbar");

    m_translateButton = m_toolbar->AddButton(ICON_LC_MOVE_3D);
    m_rotateButton = m_toolbar->AddButton(ICON_LC_ROTATE_3D);
    m_scaleButton = m_toolbar->AddButton(ICON_LC_SCALE_3D);
}

editor::GizmosUI::~GizmosUI(void)
{
    delete m_toolbar;
}

void editor::GizmosUI::Render(void)
{
    m_toolbar->Render();

    if (m_toolbar->GetButtonState(m_translateButton))
    {
        printf("Pressed translation button\n");
        
    }


    if (m_toolbar->GetButtonState(m_rotateButton))
    {
        printf("Pressed rotation button\n");

    }

    if (m_toolbar->GetButtonState(m_scaleButton))
    {
        printf("Pressed scale button\n");

    }
}
