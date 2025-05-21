#include "ui/gizmos/GizmosUI.h"
#include "ui/gizmos/TranslationGizmos.h"
#include "ui/gizmos/RotationGizmos.h"
#include "ui/gizmos/ScaleGizmos.h"

#include <engine/ui/IconsLucide.h>
#include <engine/core/components/Transform.h>
#include <engine/utility/MemoryCheck.h>

editor::GizmosUI::GizmosUI(void)
{
    m_transform = nullptr;
    m_toolbar = new ui::Toolbar("GizmosToolbar");
    m_translationGizmos = new TranslateGizmos();
    m_rotationGizmos = nullptr;// new RotationGizmos();
    m_scaleGizmos = nullptr;//new ScaleGizmos();

    m_translateButton = m_toolbar->AddButton(ICON_LC_MOVE_3D);
    m_rotateButton = m_toolbar->AddButton(ICON_LC_ROTATE_3D);
    m_scaleButton = m_toolbar->AddButton(ICON_LC_SCALE_3D);
}

editor::GizmosUI::~GizmosUI(void)
{
    delete m_scaleGizmos;
    delete m_rotationGizmos;
    delete m_translationGizmos;
    delete m_toolbar;

    m_transform = nullptr;
}

void editor::GizmosUI::Render(void)
{

    if (m_toolbar->GetButtonState(m_translateButton))
    {
        printf("Pressed translation button\n");
        m_translationGizmos->HideGizmos(false);
    }


    if (m_toolbar->GetButtonState(m_rotateButton))
    {
        printf("Pressed rotation button\n");

    }

    if (m_toolbar->GetButtonState(m_scaleButton))
    {
        printf("Pressed scale button\n");

    }
    m_toolbar->Render();
}

void editor::GizmosUI::RenderGizmos(math::Matrix4f const& viewProj)
{
    m_translationGizmos->RenderPass(viewProj);
    //m_rotationGizmos->RenderPass(viewProj);
    //m_scaleGizmos->RenderPass(viewProj);
}

void editor::GizmosUI::RenderGizmosPicking(math::Matrix4f const& viewProj)
{
    m_translationGizmos->RenderPickingPass(viewProj);
    //m_rotationGizmos->RenderPickingPass(viewProj);
    //m_scaleGizmos->RenderPickingPass(viewProj);
}

void editor::GizmosUI::UpdateGizmos(const char* wndName)
{
    if (!m_transform)
        return;

    m_translationGizmos->IsAxisSelected(wndName, m_transform);
    //if (m_translationGizmos->IsGizmosSelected())
    //{
    //    printf("e\n");
    //}
}

void editor::GizmosUI::SetSelectedTransform(engine::Transform* transform)
{
    m_transform = transform;
    m_translationGizmos->SetPosition(transform->GetPosition());
    
}