#pragma once

#include "ui/components/Component.h"
#include <engine/core/components/Renderer.h>
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

    private:
        void ModelInput(engine::Renderer* renderer);
        void ShaderInput(engine::Renderer* renderer);
    };
}