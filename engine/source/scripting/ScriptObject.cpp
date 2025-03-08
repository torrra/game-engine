#include "scripting/ScriptObject.h"
#include "core/systems/ScriptSystem.h"

namespace engine
{
	ScriptObject::ScriptObject(EntityHandle entity, const std::string& filepath)
		: m_entity(entity), m_type(filepath)
	{
		ScriptSystem::RegisterNewScriptObject(m_type, m_entity);
	}
}