#pragma once

#include "ui/components/Component.h"

namespace editor
{
    class RendererComponent : public BaseComponent
    {
    public:
        RendererComponent(void);
        ~RendererComponent(void);

    protected:
        virtual void SectionContent(void) override;
    };
}