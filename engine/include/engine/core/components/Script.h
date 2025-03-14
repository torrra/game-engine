#pragma once

#include <vector>

#include "engine/core/Component.h"
#include "engine/core/Entity.h"

#include "engine/scripting/ScriptObject.h"

namespace engine
{
	class Script : public Component
	{
	public:

		// Use parent class constructors
		using Component::Component;
		
		~Script(void) = default;

		// Call Start() in all script objects
		void Start(void);

		// Tick all script objects
		void Update(f32 deltaTime);

		// Register script component and save a reference in lua
		void Register(void) override;

		// Add a new lua script object to this component, and register it
		// in lua
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