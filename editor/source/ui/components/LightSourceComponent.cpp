#include "ui/components/LightSourceComponent.h"

#include <engine/core/components/LightSource.h>
#include <engine/ui/UIComponent.h>

namespace editor
{
    constexpr const char g_omniString[] = "Omnidirectional";
    constexpr const char g_directionalString[] = "Directional";
    constexpr const char g_spotString[] = "Spotlight";

    constexpr const char* g_typeOptions[3] =
    {
       g_omniString, g_directionalString, g_spotString
    };
}


editor::LightSourceComponent::LightSourceComponent(void)
{
    SetName("Light Source");
    SetType(LIGHT_SOURCE);
}

void editor::LightSourceComponent::SectionContent(void)
{
    if (engine::LightSource* light = GetData<engine::LightSource>())
    {
        int32 currentType = static_cast<int32>(light->GetType());

        ui::Text("Light Type");
        ui::SameLine(100.f);
        if (ui::DropDown("##Light Type", currentType, (const char**)g_typeOptions, 3))
            light->Type() = static_cast<engine::LightSource::ELightType>(currentType);
        
        ui::VerticalSpacing();
        ui::VerticalSpacing();
        ColorUI(light);
        ui::VerticalSpacing();

        ui::Text("Attenuation");
        ui::SameLine();
        InputField("##attenX", &light->Attenuation().X(), 0.f);
        InputField("##attenY", &light->Attenuation().Y(), 0.f);
        InputField("##attenZ", &light->Attenuation().Z(), 0.f);
        ui::VerticalSpacing();
        ui::VerticalSpacing();

        if (light->GetType() == engine::LightSource::ELightType::SPOTLIGHT)
        {
            ui::Text("Inner cutoff");
            ui::SameLine(80.f);
            InputField("##inner", (f32*)&light->InnerCutoff(), 0.001f);
            ui::VerticalSpacing();

            ui::Text("Outer cutoff");
            ui::SameLine(80.f);
            InputField("##outer", (f32*)&light->OuterCutoff(), 0.001f);
            ui::VerticalSpacing();
        }
    }
}

void editor::LightSourceComponent::DisplayUI(::engine::LightSource* component)
{
}

void editor::LightSourceComponent::ColorUI(::engine::LightSource* component)
{
    ui::Text("Ambient");
    ui::SameLine(80.f);
    ui::ColorEdit("##ambientPicker", (f32*)&component->AmbientColor());
    ui::VerticalSpacing();

    ui::Text("Diffuse");
    ui::SameLine(80.f);
    ui::ColorEdit("##aiffusePicker", (f32*)&component->DiffuseColor());
    ui::VerticalSpacing();

    ui::Text("Specular");
    ui::SameLine(80.f);
    ui::ColorEdit("##specularPicker", (f32*)&component->SpecularColor());
    ui::VerticalSpacing();
}
