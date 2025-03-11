#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"
#include "engine/resource/texture/Texture.h"

#include <string>

/*
*	-------- TODO: image --------
*	- Position
*	- Size
*	- Aspect Ratio
*	-----------------------------
*/

namespace engine
{
	class Image : public UIElement
	{
	public:
		ENGINE_API Image(void) = delete;
		ENGINE_API Image(const char* fileName);
		ENGINE_API virtual ~Image(void) override;

		ENGINE_API virtual void Render(void) override;

		ENGINE_API void KeepAspectRatio(bool value);

	private:
		std::string m_fileName;
		Texture* m_imageData;
		bool m_keepAspectRatio;
	};
}
