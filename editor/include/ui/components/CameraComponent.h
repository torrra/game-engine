#pragma once

#include "ui/components/Component.h"

#include <engine/CoreTypes.h>

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
    };
}