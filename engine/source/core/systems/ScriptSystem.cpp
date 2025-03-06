extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include <iostream>

#include "core/systems/ScriptSystem.h"
#include "core/SceneGraph.h"

#include "scripting/EntityScriptFunctions.h"


namespace engine
{
	SceneGraph* ScriptSystem::m_currentScene = nullptr;
	lua_State*  ScriptSystem::m_luaState = nullptr;

	// TODO: get rid of hardcoded path
	std::string ScriptSystem::m_configScriptsLocation = ScriptSystem::FindConfigScripts();
	
	void ScriptSystem::Startup(void)
	{
		m_luaState = luaL_newstate();

		if (!m_luaState)
		{
			// TODO: log error?
			std::cout << "Unable to create Lua state\n";
			exit(EXIT_FAILURE);			
		}

		luaL_openlibs(m_luaState);

		RegisterEntityFunctions(m_luaState);
		RunEntityConfigScript(m_luaState);
	}

	void ScriptSystem::Shutdown(void)
	{
		lua_close(m_luaState);
	}

	void ScriptSystem::SetCurrentScene(SceneGraph* graph)
	{
		m_currentScene = graph;
	}

	SceneGraph* ScriptSystem::GetCurrentScene(void)
	{
		return m_currentScene;
	}

	const std::string& ScriptSystem::GetConfigScriptsLocation(void)
	{
		return m_configScriptsLocation;
	}

	std::string ScriptSystem::FindConfigScripts(void)
	{
		FilePath currentPath = std::filesystem::current_path();
		FilePath parentPath = currentPath.parent_path();

		if (std::filesystem::is_directory(currentPath.append("scripts\\")))
			return parentPath.string();

		else
			return parentPath.append("scripts\\").string();
	}

}