#pragma once

#include "engine/ui/UIElement.h"
#include "engine/EngineExport.h"

#include <string>

namespace engine
{
	class Label : public UIElement
	{
	public:
		ENGINE_API Label(const char* text);
		ENGINE_API virtual ~Label(void) override;

		ENGINE_API virtual void Render(void) override;

	private:
		math::Vector4f m_textColor;
		std::string m_text;

	};
}