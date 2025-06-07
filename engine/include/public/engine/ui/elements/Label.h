#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"
#include "engine/utility/Colors.h"

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
    // Text UI element
    class Label : public UIElement
    {
    public:

        using UIElement::UIElement;

        ENGINE_API			Label(void) = delete;
        ENGINE_API			Label(const char* text, math::Vector2f const& position);
        ENGINE_API virtual ~Label(void) override = default;

        virtual void		Render(void) override;
        
        ENGINE_API void		SetTextColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
        ENGINE_API void		SetFont(const char* fontName, f32 fontSize);
        ENGINE_API void		WrapText(f32 maxWidthPx);

    private:
        std::string		m_text;
        ImFont*			m_font;
        f32				m_fontScale;
        f32				m_textBoxWidth;
        uint32			m_textColor;
    };
}