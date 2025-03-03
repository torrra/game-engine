#pragma once

#include "CoreTypes.h"

namespace engine
{

	namespace ecs
	{
		enum EFlags : uint64
		{
			NONE = 0,
			INVALID_OBJECT = 1,
			INACTIVE_OBJECT = (uint64)(1 << 1)
		};
	}


	using EntityHandle = uint64;

	class Component;
	class Entity;

	template <typename TComponentType>
	concept CValidComponent = std::derived_from<TComponentType, Component>;



	class Transform;

}