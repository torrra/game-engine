#include "ui/components/RendererComponent.h"

#include <engine/ui/UIComponent.h>

editor::RendererComponent::RendererComponent(void)
    : m_renderer(nullptr)
{
    SetName("Renderer");
}

editor::RendererComponent::~RendererComponent(void)
{
    m_renderer = nullptr;
}

void editor::RendererComponent::SetRenderer(engine::Renderer* renderer)
{
    m_renderer = renderer;
}

void editor::RendererComponent::SectionContent(void)
{
    ui::Text("Model: to do"); // TODO: add input for model (need asset window)
    ui::Text("Shader: to do"); // TODO: add input for shader (need asset window)
}
