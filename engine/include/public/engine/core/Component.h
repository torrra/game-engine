#pragma once

#include <type_traits>
#include <unordered_map>

#include "TypesECS.h"
#include "Entity.h"

namespace engine
{
	class Component
	{
	private:

		template<CValidComponent TComponentType>
		using IndexedComponent = std::pair<uint64, TComponentType>;

		template<CValidComponent TComponentType>
		using DeserializedArray = std::vector<std::pair<uint64, TComponentType>>;

		friend class SceneGraph;

	public:

		ENGINE_API Component(void) = default;
		ENGINE_API Component(EntityHandle owner, class SceneGraph* scene);
		ENGINE_API virtual ~Component(void) = default;

		virtual void Register(void) = 0;

		// Is this component a valid object?
		// true: this object is a valid component instance
		// false: this object is 'dead' and subject to overwrite
		ENGINE_API
		bool IsValid(void) const;

		// Is this component included when updating the scene?
		ENGINE_API
		bool IsActive(void) const;

		// Set the component's active status
		// true: will be included in update
		// false: will be excluded in update
		ENGINE_API
		void Activate(bool activeState);

		// Permanently set object up for destruction
		ENGINE_API
		void Invalidate(void);

		ENGINE_API
		EntityHandle GetOwner(void) const;

		ENGINE_API
		virtual	void SerializeText(std::ofstream&,
								   EntityHandle,
								   uint64) const {}

		template <CValidComponent TComponentType>
		static void DeserializeComponentText(DeserializedArray<TComponentType>& array,
											std::ifstream& file);

	protected:

		ENGINE_API
		void DeserializeIndexedText(std::ifstream& input, uint64& index);

		ENGINE_API
		virtual void DeserializeText(std::ifstream&) {}

		// which entity in scene graph owns this component
		EntityHandle	m_owner = static_cast<EntityHandle>(-1);

		// Scene graph in which ths component exists
		class SceneGraph* m_currentScene = nullptr;

		// status modifiers that affect behavior (inactive, invalid)
		uint64			m_flags = 0;
	};


	// Defines whether a type has objects that NEED to be updated after their parents
	// e.g. a transform component should be updated after its owner's transform.
	// False by default, must be manually instantiated and defined as true if needed
	template <CValidComponent TComponentType>
	struct UpdateAfterParent
	{
		static constexpr bool m_value = false;
	};

	// Defines if a component type should be updated each tick in the main loop
	// False by default, must be manually instantiated and defined as true if needed
	template <CValidComponent TComponentType>
	struct UpdateComponent
	{
		static constexpr bool m_value = false;
	};

	template<CValidComponent TComponentType>
	inline void Component::DeserializeComponentText(DeserializedArray<TComponentType>& array,
													std::ifstream& file)
	{
		constexpr EntityHandle invalid = static_cast<EntityHandle>(-1);

		IndexedComponent<TComponentType>& component = array.emplace_back(invalid, TComponentType(invalid, nullptr));

		if constexpr (UpdateAfterParent<TComponentType>::m_value)
			component.second.DeserializeIndexedText(file, component.first);

		else
			component.second.DeserializeText(file);
	}

}