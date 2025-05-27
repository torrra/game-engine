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

        void RenderMaterialList(engine::Renderer* renderer);
        void MaterialInput(engine::Renderer* renderer, const std::string& name, uint32 index);

        std::string m_modelName;
        std::string m_fragName;
    };
}