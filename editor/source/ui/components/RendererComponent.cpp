#include "ui/components/RendererComponent.h"

#include <engine/ui/UIComponent.h>

editor::RendererComponent::RendererComponent(void)
{
    SetName("Renderer");
}

editor::RendererComponent::~RendererComponent(void)
{}

void editor::RendererComponent::SectionContent(void)
{
    ui::Text("e");
}
