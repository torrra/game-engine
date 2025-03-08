#include "CoreTypes.h"

#include "core/components/Script.h"
#include "core/systems/ScriptSystem.h"

#include <iostream>

namespace engine
{
	void Script::Start(void)
	{
		ScriptSystem::StartScript(m_owner);
	}

	void Script::Update(float deltaTime)
	{
		ScriptSystem::UpdateScript(m_owner, deltaTime);
	}

	void Script::Register(void)
	{
		ScriptSystem::RegisterNewScriptComponent(m_owner);
	}

	void Script::AddScriptObject(const std::string& type)
	{
		if (type.find(".lua") == std::string::npos)
		{
			std::cout << "[Script component]: " << type << " is not a valid lua script.\n";
			return;
		}

		uint64 dotLocation = 0;

		while (type[dotLocation] != '.')
			++dotLocation;

		std::string formattedType = type.substr(0, dotLocation);

		for (char& character : formattedType)
		{
			if (character >= 'A' && character <= 'Z')
				character += 32;
		}

		m_scriptObjects.emplace_back(m_owner, formattedType);
	}
}