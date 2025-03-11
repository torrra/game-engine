#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"

#include <string>

/*
*	------------------ TODO: label ------------------
*	- Alignment (left, right, center)		
*	- Options: bold, italic, underline		
*	-------------------------------------------------
*/

struct ImFont;

namespace engine
{
	class Label : public UIElement
	{
	public:
		ENGINE_API Label(void) = delete;
		ENGINE_API Label(const char* text);
		ENGINE_API virtual ~Label(void) override;

		ENGINE_API virtual void Render(void) override;
		
		ENGINE_API void SetTextColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
		ENGINE_API void SetFont(const char* fontName, f32 fontSize);
		ENGINE_API void WrapText(f32 maxWidthPx);

	private:
		std::string m_text;
		math::Vector4f m_textColor;
		ImFont* m_font;
		f32 m_fontScale;
		f32 m_textBoxWidth;
	};
}