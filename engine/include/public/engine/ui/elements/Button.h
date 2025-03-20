#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"

#include <string>

namespace engine
{
	/*
		UI Button States
		Button up: not interacting with button
		Button hovered: cursor hovering over button
		Button Pressed: Mouse click on button
	*/
	enum EButtonState : uint8
	{
		BUTTON_UP,
		BUTTON_HOVERED,
		BUTTON_PRESSED
	};

	class Button : public UIElement
	{
	public:
		ENGINE_API				Button(void) = delete;
		ENGINE_API				Button(const char* text, math::Vector2f const& position, math::Vector2f const& size);
		ENGINE_API virtual		~Button(void) override = default;

		virtual void			Render(void) override;

		ENGINE_API void			SetText(const char* text);
		ENGINE_API void			SetColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
		ENGINE_API void			SetHoveredColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
		ENGINE_API void			SetPressedColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
		ENGINE_API void			SetRounding(f32 rounding);
		ENGINE_API void			SetPadding(math::Vector2<f32> paddingPx);

		ENGINE_API EButtonState	GetState(void) const noexcept;
		ENGINE_API bool			IsPressed(void) const noexcept;
	
	private:
		void	CalcSize(void);

		std::string				m_text;
		math::Vector2<f32>		m_padding;
		uint32					m_regularColor;
		uint32					m_hoveredColor;
		uint32					m_pressedColor;
		f32						m_rounding;
		EButtonState			m_state;
	};
}