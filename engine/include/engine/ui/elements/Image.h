#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"
#include "engine/resource/texture/Texture.h"

#include <string>

namespace engine
{
	class Image : public UIElement
	{
	public:
		ENGINE_API			Image(const char* fileName, math::Vector2f position);
		ENGINE_API			Image(void) = delete;
		ENGINE_API virtual	~Image(void) override = default;

		virtual void		Render(void) override;

		ENGINE_API void		ConstantAspectRatio(bool value);
		ENGINE_API void		SetBorderColor(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
		ENGINE_API void		SetImageTint(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);

		// Function to check if image data is loaded
		bool IsDataValid(void);

	private:
		std::string m_fileName;
		Texture*	m_data;
		uint32		m_borderColor;
		uint32		m_tint;
		bool		m_keepAspectRatio;
	};
}
