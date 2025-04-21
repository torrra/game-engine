#include "ui/components/CameraComponent.h"

#include <engine/ui/UIComponent.h>

editor::CameraComponent::CameraComponent(void)
{
    SetName("Camera");
}

void editor::CameraComponent::SectionContent(void)
{
    ui::Text("e");
}
