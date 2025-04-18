#pragma once

#include "ui/components/Component.h"
#include <engine/core/components/Renderer.h>
#include <engine/CoreTypes.h>

namespace editor
{
    class RendererComponent : public BaseComponent
    {
    public:
        RendererComponent(void);
        ~RendererComponent(void);

        void SetRenderer(engine::Renderer* renderer);
    protected:
        virtual void SectionContent(void) override;

    private:
        engine::Renderer* m_renderer;
    };
}