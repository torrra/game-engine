#pragma once
#include "engine/CoreTypes.h"

namespace engine
{
	class IResource
	{
	protected:

		// This enum only applies to resources loaded on a thread other
		// than the main thread
		enum ELoadStatus : uint8
		{
			// Resource was successfully loaded from another thread
			LOADED = 1,

			// Necessary graphics API calls were made
			GRAPHICS_CALLS_COMPLETE = (uint8) (1 << 1)
		};

	public:
						IResource(void) = default;
		virtual			~IResource(void) = default;

		virtual void	LoadResource(const char* filePath) = 0;
	};
}