#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"
#include "engine/utility/Colors.h"

#include <math/Vector2.hpp>

namespace engine
{
	class ProgressBar : public UIElement
	{
	public:
		ENGINE_API			ProgressBar(void) = delete;
		ENGINE_API			ProgressBar(math::Vector2f const& position, math::Vector2f const& size, math::Vector2f const& range);
		ENGINE_API virtual ~ProgressBar(void) override = default;

		virtual void		Render(void) override;

		ENGINE_API void		SetValue(f32 value);
		ENGINE_API void		SetBgColor(f32 red, f32 green, f32 blue, f32 alpha);
		ENGINE_API void		SetFillColor(f32 red, f32 green, f32 blue, f32 alpha);
	
	private:
		math::Vector2f		m_range;
		f32					m_value;
		uint32				m_bgColor;
		uint32				m_fillColor;

	};
}