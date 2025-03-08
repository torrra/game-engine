#pragma once

#include <string>

#include "engine/core/TypesECS.h"

namespace engine
{
	class ScriptObject
	{
	public:

		ScriptObject(void) = delete;
		ScriptObject(EntityHandle entity, const std::string& type);
		ScriptObject(const ScriptObject&) = default;
		~ScriptObject(void) = default;

	private:

		// TODO: change for script resource when ready
		std::string		m_type;
		EntityHandle	m_entity;
	};
}