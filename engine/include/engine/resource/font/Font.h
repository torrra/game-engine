#pragma once

#include "engine/resource/Resource.h"
#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

struct ImFont;

namespace engine
{
	class Font : public IResource
	{
	public:
		ENGINE_API Font(void);
		ENGINE_API virtual ~Font(void) = default;

		ENGINE_API virtual void LoadResource(const char* fileName) override;

		ENGINE_API ImFont* GetFont(void) const noexcept;
		ENGINE_API f32 GetFontSize(void) const noexcept;
	private:
		ImFont* m_font;
	};
}