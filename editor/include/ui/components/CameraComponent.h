#pragma once

#include "ui/components/Component.h"

namespace editor
{
    class CameraComponent : public BaseComponent
    {
    public:
        CameraComponent(void);
        ~CameraComponent(void) = default;

    protected:
        virtual void SectionContent(void) override;
    };
}