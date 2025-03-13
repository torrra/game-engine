#pragma once

namespace engine
{
	class IResource
	{
	public:
						IResource(void) = default;
		virtual			~IResource(void) = default;

		virtual void	LoadResource(const char* filePath) = 0;
	};
}