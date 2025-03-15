#pragma once

#include "engine/ui/UIElement.h"
#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include "math/Vector2.hpp"
#include "math/Vector4.hpp"

namespace engine
{
	class Rectangle : public UIElement
	{
	public:
		ENGINE_API Rectangle(void) = delete;
		ENGINE_API Rectangle(math::Vector2f const& pos, math::Vector2f const& sizePx);
		ENGINE_API virtual ~Rectangle(void) override = default;

		ENGINE_API virtual void Render(void) override;

		ENGINE_API void SetColor(f32 red, f32 green, f32 blue, f32 alpha);
		ENGINE_API void SetRounding(f32 rounding);


	private:
		f32 m_rounding;
		uint32 m_color;
	};
}