#pragma once

#include "ui/components/Component.h"

#include <engine/CoreTypes.h>

namespace editor
{
    class RendererComponent : public BaseComponent
    {
    public:
        RendererComponent(void);
        ~RendererComponent(void);

    protected:
        virtual void SectionContent(void) override;

    private:
    };
}