#pragma once

#include "ui/components/Component.h"

namespace editor
{
    class CameraComponent : public BaseComponent
    {
    public:
        CameraComponent(void);
        ~CameraComponent(void);

    protected:
        virtual void SectionContent(void) override;

    private:
        f32 m_fov;
        f32 m_near;
        f32 m_far;
    };
}