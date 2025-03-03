#pragma once

#include "Component.h"

namespace engine
{
	template <CValidComponent TComponentType>
	class ComponentArray
	{
	public:

		void MoveReparentedComponent(EntityHandle owner);

	private:

		std::unordered_map<EntityHandle, uint64>	m_entityIndexMap;
		std::vector<TComponentType>					m_components;
	};


	template<CValidComponent TComponentType>
	inline void ComponentArray<TComponentType>::MoveReparentedComponent(EntityHandle owner)
	{
		if constexpr (!UpdateAfterParent<TComponentType>::m_value)
		{
			printf("not moving component");
			return;
		}

		printf("yea we moving the component\n");
	}
}