#pragma once

#include "engine/ui/UIElement.h"
#include "engine/ui/elements/Label.h"
#include "engine/ui/elements/Image.h"
#include "engine/ui/elements/ProgressBar.h"
#include "engine/ui/elements/Button.h"
#include "engine/ui/elements/Rect.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#include <vector>
#include <string>

namespace engine
{
	class Canvas
	{
	public:
		ENGINE_API Canvas(math::Vector2f const& windowSize, math::Vector4f const& color);
		ENGINE_API ~Canvas(void) = default;

		ENGINE_API void Render(void);
		ENGINE_API void Clear(void);

		ENGINE_API Label* AddLabel(const char* text);
		ENGINE_API Image* AddImage(const char* fileName);
		ENGINE_API Button* AddButton(const char* text, std::function<void(void)> function);
		ENGINE_API ProgressBar* AddProgressBar(math::Vector2f const& position, math::Vector2f const& size, math::Vector2f const& range);
		ENGINE_API Rectangle* AddRectangle(math::Vector2f const& pos, math::Vector2f const& sizePx);

	private:
		void RescaleCanvas(void);

		math::Vector2f m_size;
		math::Vector2f m_prevSize;
		std::vector<UIElement*> m_elements;
		math::Vector4<float> m_color;
		int32 m_uid;
	};
}