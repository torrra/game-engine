#pragma once

#include "engine/ui/UIElement.h"
#include "engine/ui/elements/Label.h"
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
		ENGINE_API Canvas(math::Vector4<float> color);
		ENGINE_API ~Canvas(void) = default;

		ENGINE_API void Render(void);
		ENGINE_API void Clear(void);

		ENGINE_API Label* AddLabel(const char* text);

	private:
		std::vector<UIElement*> m_elements;
		math::Vector4<float> m_color;
	};
}