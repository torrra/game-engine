#pragma once

#include "Component.h"

namespace engine
{
	template <CValidComponent TComponentType>
	class ComponentArray
	{
	public:

		void			MoveReparentedComponent(EntityHandle owner);
		void			InvalidateComponent(EntityHandle owner);
		TComponentType* CreateComponent(EntityHandle owner, EntityHandle parent);
		TComponentType* GetComponent(EntityHandle owner);

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

		if ((!m_entityIndexMap.contains(owner)) || m_components.empty())
			return;

		uint64				newIndex = m_components.size();
		TComponentType&		toMove = m_components[m_entityIndexMap[owner]];

		m_components.emplace_back(toMove);
		toMove.Invalidate();

	}

	template<CValidComponent TComponentType>
	inline void ComponentArray<TComponentType>::InvalidateComponent(EntityHandle owner)
	{
		m_components[m_entityIndexMap[owner]].Invalidate();
		m_entityIndexMap.erase(owner);
	}

	template<CValidComponent TComponentType>
	inline TComponentType* ComponentArray<TComponentType>::CreateComponent(EntityHandle owner, EntityHandle parent)
	{		
		EntityHandle    parentIndex = 0;

		if (m_entityIndexMap.contains(parent))
			parentIndex = m_entityIndexMap[parent];

		for (EntityHandle newIndex = 0; newIndex < m_components.size(); ++newIndex)
		{
			TComponentType& currentComponent = m_components[newIndex];

			if (currentComponent.IsValid() || parentIndex > newIndex)
				continue;

			printf("[Component array]: filling invalid slot\n");

			currentComponent = TComponentType(owner);
			m_entityIndexMap[owner] = newIndex;		
			return &currentComponent;
		}

		printf("[Component array]: creating new slot\n");
		m_entityIndexMap[m_components.size()] = owner;
		return &m_components.emplace_back(owner);
	}

	template<CValidComponent TComponentType>
	inline TComponentType* ComponentArray<TComponentType>::GetComponent(EntityHandle owner)
	{
		if (!m_entityIndexMap.contains(owner))
			return nullptr;

		return &m_components[m_entityIndexMap[owner]];
	}
}