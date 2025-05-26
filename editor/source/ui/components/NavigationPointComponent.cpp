#include "ui/components/NavigationPointComponent.h"

#pragma region Engine

#include <engine/ui/UIComponent.h>
#include <engine/ui/UIStyle.h>
#include <engine/Engine.h>

#pragma endregion

editor::NavigationPointComponent::NavigationPointComponent(void)
{
    SetName("NavigationPoint");
    SetType(NAVIGATION_POINT);
}

void editor::NavigationPointComponent::SectionContent(void)
{
    if (engine::NavigationPoint* navPoint = GetData<engine::NavigationPoint>())
    {
        math::Vector3f position = navPoint->GetPosition();
        ui::Text("NavPoint Position: ");
        InputField("##X", &position[0], 0.05f);
        InputField("##Y", &position[1], 0.05f);
        InputField("##Z", &position[2], 0.05f);
        navPoint->SetPosition(position);
        ui::VerticalSpacing();
    }
}
