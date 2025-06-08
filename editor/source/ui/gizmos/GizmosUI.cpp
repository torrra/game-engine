#include "ui/gizmos/GizmosUI.h"
#include "ui/gizmos/TranslationGizmos.h"
#include "ui/gizmos/RotationGizmos.h"
#include "ui/gizmos/ScaleGizmos.h"

#include <engine/ui/IconsLucide.h>
#include <engine/core/components/Transform.h>
#include <engine/utility/MemoryCheck.h>

editor::GizmosUI::GizmosUI(void)
{
    m_toolbar = new ui::Toolbar("GizmosToolbar");
    m_translationGizmos = new TranslateGizmos();
    m_rotationGizmos = new RotationGizmos();
    m_scaleGizmos = new ScaleGizmos();
    m_currentGizmos = NO_GIZMOS;
    m_transform = nullptr;

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

    DeselectEntity();
}

void editor::GizmosUI::Render(void)
{
    // Render UI
    m_toolbar->Render();

    // Translate gizmos button
    if (m_toolbar->GetButtonState(m_translateButton))
    {
        bool isGizmosSelected = (m_currentGizmos == TRANSLATION_GIZMOS);

        m_toolbar->SetSelected((isGizmosSelected) ? -1 : m_translateButton);
        m_currentGizmos = (isGizmosSelected) ? NO_GIZMOS : TRANSLATION_GIZMOS;
        m_translationGizmos->HideGizmos(m_currentGizmos == NO_GIZMOS);
    }

    // Rotate gizmos button
    if (m_toolbar->GetButtonState(m_rotateButton))
    {
        bool isGizmosSelected = (m_currentGizmos == ROTATION_GIZMOS);
        
        m_toolbar->SetSelected((isGizmosSelected) ? -1 : m_rotateButton);
        m_currentGizmos = (isGizmosSelected) ? NO_GIZMOS : ROTATION_GIZMOS;
        m_rotationGizmos->HideGizmos(m_currentGizmos == NO_GIZMOS);
    }

    // Scale gizmos button
    if (m_toolbar->GetButtonState(m_scaleButton))
    {
        bool isGizmosSelected = (m_currentGizmos == SCALE_GIZMOS);
        
        m_toolbar->SetSelected((isGizmosSelected) ? -1 : m_scaleButton);
        m_currentGizmos = (isGizmosSelected) ? NO_GIZMOS : SCALE_GIZMOS;
        m_scaleGizmos->HideGizmos(m_currentGizmos == NO_GIZMOS);
    }
}

void editor::GizmosUI::RenderGizmos(math::Matrix4f const& viewProj, math::Vector3f const& cameraPos)
{
    if (!m_transform)
        return;

    switch (m_currentGizmos)
    {
    case editor::TRANSLATION_GIZMOS:
        m_translationGizmos->RenderPass(viewProj, cameraPos);
        break;
    case editor::ROTATION_GIZMOS:
        m_rotationGizmos->RenderPass(viewProj, cameraPos);
        break;
    case editor::SCALE_GIZMOS:
        m_scaleGizmos->RenderPass(viewProj, cameraPos);
        break;
    case editor::NO_GIZMOS:
    default:
        break;
    }
}

void editor::GizmosUI::RenderGizmosPicking(math::Matrix4f const& viewProj)
{
    if (!m_transform)
        return;

    switch (m_currentGizmos)
    {
    case editor::TRANSLATION_GIZMOS:
        m_translationGizmos->RenderPickingPass(viewProj);
        break;
    case editor::ROTATION_GIZMOS:
        m_rotationGizmos->RenderPickingPass(viewProj);
        break;
    case editor::SCALE_GIZMOS:
        m_scaleGizmos->RenderPickingPass(viewProj);
        break;
    case editor::NO_GIZMOS:
    default:
        break;
    }
}

void editor::GizmosUI::UpdateGizmos(const char* wndName)
{
    if (!m_transform)
        return;

    switch (m_currentGizmos)
    {
    case editor::TRANSLATION_GIZMOS:
        m_translationGizmos->SetPosition(m_transform->GetPosition());
        m_translationGizmos->IsAxisSelected(wndName, m_transform);
        break;
    case editor::ROTATION_GIZMOS:
        m_rotationGizmos->SetPosition(m_transform->GetPosition());
        m_rotationGizmos->IsAxisSelected(wndName, m_transform);
        break;
    case editor::SCALE_GIZMOS:
        m_scaleGizmos->SetPosition(m_transform->GetPosition());
        m_scaleGizmos->IsAxisSelected(wndName, m_transform);
        break;
    case editor::NO_GIZMOS:
    default:
        break;
    }
}

void editor::GizmosUI::DeselectEntity(void)
{
    // Unselect to reset button and gizmos state
    m_toolbar->SetSelected(-1);
    m_currentGizmos = NO_GIZMOS;
    m_transform = nullptr;

    // Ensure all gizmos are not visible
    m_translationGizmos->HideGizmos(true);
    m_rotationGizmos->HideGizmos(true);
    m_scaleGizmos->HideGizmos(true);
}

void editor::GizmosUI::SetSelectedTransform(engine::Transform* transform)
{
    m_transform = transform;
}
