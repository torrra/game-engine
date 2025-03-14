#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"

#include <string>
#include <functional>

/*
	-------- TODO: Button --------
	- Regular color
	- Hovered color
	- Pressed color
	- Text
	- Image
	- Rounding
	------------------------------
*/

namespace engine
{
	class Button : public UIElement
	{
	public:
		ENGINE_API Button(void) = delete;
		ENGINE_API Button(const char* text, std::function<void(void)> function);
		ENGINE_API virtual ~Button(void) override = default;
		
		ENGINE_API virtual void Render(void) override;
	
		ENGINE_API void SetText(const char* text);
		ENGINE_API void SetColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
		ENGINE_API void SetHoveredColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
		ENGINE_API void SetPressedColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
		ENGINE_API void SetRounding(f32 rounding);
		ENGINE_API void SetPadding(math::Vector2<uint16> paddingPx);
	
	private:
		void CalcSize(void);
		math::Vector2f CalcTextSize(uint16 fontSize);


		std::function<void(void)> m_action;
		std::string m_text;
		uint32 m_regularColor;
		uint32 m_hoveredColor;
		uint32 m_pressedColor;
		f32 m_edgeRounding;
		math::Vector2<uint16> m_padding;

	};
}