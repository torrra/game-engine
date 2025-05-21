#pragma once

#include <engine/ui/UIToolbar.h>
#include <engine/CoreTypes.h>
#include <math/Matrix4.hpp>

namespace engine
{
    class Transform;
}

namespace editor
{
    enum ESelectedGizmos
    {
        NO_GIZMOS,
        TRANSLATION_GIZMOS,
        ROTATION_GIZMOS,
        SCALE_GIZMOS
    };

    class GizmosUI
    {
    public:
        GizmosUI(void);
        ~GizmosUI(void);

        void Render(void);
        void RenderGizmos(math::Matrix4f const& viewProj);
        void RenderGizmosPicking(math::Matrix4f const& viewProj);
        void UpdateGizmos(const char* wndName);

        void SetSelectedTransform(engine::Transform* transform);

    private:
        ui::Toolbar* m_toolbar;
        class TranslateGizmos* m_translationGizmos;
        class RotationGizmos* m_rotationGizmos;
        class ScaleGizmos* m_scaleGizmos;
        class engine::Transform* m_transform;
        uint32 m_translateButton;
        uint32 m_rotateButton;
        uint32 m_scaleButton;
        ESelectedGizmos m_currentGizmos;

        friend class EditorApplication;
    };
}