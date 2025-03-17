#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"
#include "engine/ui/elements/Label.h"
#include "engine/ui/elements/Image.h"
#include "engine/ui/elements/ProgressBar.h"
#include "engine/ui/elements/Button.h"
#include "engine/ui/elements/Rect.h"

#include <math/Vector2.hpp>

#include <vector>

namespace engine
{
	/*
		Canvas class
		The canvas class is used for adding UI elements to the screen. It should
		only be used for 'in-game' UI.
	*/
	class Canvas
	{
	public:
		/*
			Create a canvas

			Window size: size in pixels
			Color: background color as RGBA all values are between 0 - 1
		*/
		ENGINE_API				Canvas(math::Vector2f const& windowSize, math::Vector4f const& color);
		ENGINE_API				Canvas(void) = delete;
		ENGINE_API				~Canvas(void) = default;

		// Render function used to display UI to screen. It should be placed at the end of the update loop.
		ENGINE_API void			Render(void);

		// Clear function used to destroy a canvas & clear all associated UI elements.
		ENGINE_API void			Clear(void);

		// Add UI elements
		ENGINE_API Label*		AddLabel(const char* text); 
		ENGINE_API Image*		AddImage(const char* fileName);
		ENGINE_API Button*		AddButton(const char* text, std::function<void(void)> function);
		ENGINE_API ProgressBar* AddProgressBar(math::Vector2f const& position, math::Vector2f const& size, math::Vector2f const& range);
		ENGINE_API Rectangle*	AddRectangle(math::Vector2f const& position, math::Vector2f const& size);

		// Change canvas's background color. All values between 0 - 1.
		ENGINE_API void			SetColor(f32 red, f32 green, f32 blue, f32 alpha);

	private:
		void					RescaleCanvas(void);

		std::vector<UIElement*> m_elements;
		math::Vector2f			m_size;
		math::Vector2f			m_prevSize;
		uint32					m_bgColor;
		int32					m_uidCounter;
	};
}