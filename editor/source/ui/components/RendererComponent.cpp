#include "ui/components/RendererComponent.h"
#include <engine/core/components/Renderer.h>
#include <engine/ui/UIComponent.h>

editor::RendererComponent::RendererComponent(void)
{
    SetName("Renderer");
}

editor::RendererComponent::~RendererComponent(void)
{
    SetData<engine::Renderer>(nullptr);
}

void editor::RendererComponent::SectionContent(void)
{
    ui::Text("Model: to do"); // TODO: add input for model (need asset window)
    ui::Text("Shader: to do"); // TODO: add input for shader (need asset window)
}
