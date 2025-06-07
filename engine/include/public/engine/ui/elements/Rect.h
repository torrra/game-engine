#pragma once

#include "engine/ui/UIElement.h"
#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include "math/Vector2.hpp"

namespace engine
{
    // UI rectangle shapes
    class Rectangle : public UIElement
    {
    public:

        using UIElement::UIElement;

        ENGINE_API			Rectangle(void) = delete;
        ENGINE_API			Rectangle(math::Vector2f const& position, math::Vector2f const& size);
        ENGINE_API virtual ~Rectangle(void) override = default;

        virtual void		Render(void) override;

        ENGINE_API void		SetColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
        ENGINE_API void		SetRounding(f32 rounding);

    private:
        f32		m_rounding;
        uint32	m_color;
    };
}