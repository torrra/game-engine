#pragma once

#include "ui/components/Component.h"
#include <engine/core/components/Camera.h>

namespace editor
{
    class CameraComponent : public BaseComponent
    {
    public:
        CameraComponent(void);
        ~CameraComponent(void);

        void SetCamera(engine::Camera* camera);

    protected:
        virtual void SectionContent(void) override;

    private:
        engine::Camera* m_camera;

        f32 m_fov;
        f32 m_near;
        f32 m_far;
    };
}