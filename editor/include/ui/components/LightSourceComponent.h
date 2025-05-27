#pragma once

#include "ui/components/Component.h"

namespace editor
{
    class LightSourceComponent final : public BaseComponent
    {
    public:

        LightSourceComponent(void);
        LightSourceComponent(const LightSourceComponent&) = delete;

        LightSourceComponent(LightSourceComponent&&) = default;
        ~LightSourceComponent(void) = default;

    private:

        void SectionContent(void) override;

        void DisplayUI(class ::engine::LightSource* component);
        void ColorUI(class ::engine::LightSource* component);

    };
}