#pragma once

#include <vector>

#include "core/Component.h"
#include "core/Entity.h"

#include "scripting/ScriptObject.h"

namespace engine
{
	class Script : public Component
	{
	public:

		using Component::Component;
		
		~Script(void) = default;

		void Start(void);
		void Update(float deltaTime);
		void Register(void) override;

		void AddScriptObject(const std::string& type);

	private:

		std::vector<ScriptObject> m_scriptObjects;
		
	};

	template <>
	struct UpdateAfterParent<Script>
	{
		static constexpr bool m_value = true;
	};

	template <>
	struct UpdateComponent<Script>
	{
		static constexpr bool m_value = true;
	};

	template<>
	inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<Script>()
	{
		return SCRIPT;
	}

}