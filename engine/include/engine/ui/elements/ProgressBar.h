#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/ui/UIElement.h"

#include <string>

namespace engine
{
	class ProgressBar : public UIElement
	{
	public:
		ENGINE_API ProgressBar(void);
		ENGINE_API virtual ~ProgressBar(void) override;

		ENGINE_API virtual void Render(void) override;
	private:

	};
}