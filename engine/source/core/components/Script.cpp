#include "CoreTypes.h"

#include "core/components/Script.h"
#include "core/systems/ScriptSystem.h"

#include "serialization/TextSerializer.h"

#include <iostream>

namespace engine
{
	void Script::Start(void)
	{
		ScriptSystem::StartScript(m_owner);
	}

	void Script::Update(f32 deltaTime)
	{
		ScriptSystem::UpdateScript(m_owner, deltaTime);
	}

	void Script::Register(void)
	{
		ScriptSystem::RegisterNewComponent("_NewScriptComponent", m_owner);

		for (const ScriptObject& object : m_scriptObjects)
			object.Register();
	}

	void Script::AddScriptObject(const std::string& type)
	{
		std::string formattedType = type;

		for (char& character : formattedType)
		{
			if (character >= 'A' && character <= 'Z')
				character += 32;
		}

		m_scriptObjects.emplace_back(m_owner, formattedType);
	}

	void Script::SerializeText(std::ofstream& output, EntityHandle owner, uint64 index) const
	{
		output << "[Script]\n   ";

		if constexpr (UpdateAfterParent<Script>::m_value)
		{
			text::Serialize(output, "index", index);
			output << "\n   ";
		}

		text::Serialize(output, "owner", owner);
		output << "\n   ";

		text::Serialize(output, "count", m_scriptObjects.size());
		output << "\n   ";

		for (const ScriptObject& object : m_scriptObjects)
		{
			text::Serialize(output, "scriptObject", object.GetType());
			output << "\n   ";
		}

		text::Serialize(output, "flags", m_flags);
		output << '\n';

	}

	void Script::DeserializeText(std::ifstream& /*input*/)
	{
		
	}
}