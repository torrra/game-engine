#pragma once

#include "ui/components/Component.h"

#include <string>

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