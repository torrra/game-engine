extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include <iostream>

#include "core/components/Script.h"
#include "core/systems/ScriptSystem.h"
#include "core/SceneGraph.h"

#include "scripting/EntityScriptFunctions.h"
#include "scripting/ComponentFunctions.h"
#include "scripting/ScriptFunctions.h"

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
		RegisterComponentFunctions(m_luaState);
		RegisterScriptComponentFunctions(m_luaState);

		RunConfigScript("Component.lua");
		RunConfigScript("Entity.lua");

		RunConfigScript("ScriptObject.lua");
		RunConfigScript("Script.lua");
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

	void ScriptSystem::RegisterNewEntity(EntityHandle newEntity, const std::string& name)
	{
		if (name.empty())
			return;

		lua_getglobal(m_luaState, "_AddEntityHandle");
		lua_pushinteger(m_luaState, newEntity);
		lua_pushstring(m_luaState, name.c_str());

		if (lua_pcall(m_luaState, 2, 0, 0) != LUA_OK)
			LogLuaError();
	}

	void ScriptSystem::RegisterNewScriptComponent(EntityHandle owner)
	{
		lua_getglobal(m_luaState, "_NewScriptComponent");
		lua_pushinteger(m_luaState, owner);
		  
		if (lua_pcall(m_luaState, 1, 0, 0) != LUA_OK)
			LogLuaError();
	}

	void ScriptSystem::RegisterNewScriptObject(const std::string& type, EntityHandle owner)
	{
		lua_getglobal(m_luaState, "_NewScriptObject");
		lua_pushstring(m_luaState, type.c_str());
		lua_pushinteger(m_luaState, owner);

		if (lua_pcall(m_luaState, 2, 0, 0) != LUA_OK)
			LogLuaError();
	}

	std::string ScriptSystem::FindConfigScripts(void)
	{
		// scripts/ directory should either be in child or sibling folder
		// (relative to the executable's working directory)
		FilePath currentPath = std::filesystem::current_path();
		FilePath parentPath = currentPath.parent_path();

		// look for child folder
		if (std::filesystem::is_directory(currentPath.append("scripts\\")))
			return parentPath.string();

		// default to sibling folder
		else
			return parentPath.append("scripts\\").string();
	}

	void ScriptSystem::StartScript(EntityHandle entity)
	{
		lua_getglobal(m_luaState, "_StartScript");
		lua_pushinteger(m_luaState, entity);

		if (lua_pcall(m_luaState, 1, 0, 0) != LUA_OK)
			LogLuaError();
	}

	void ScriptSystem::UpdateScript(EntityHandle entity, float deltaTime)
	{
		lua_getglobal(m_luaState, "_UpdateScript");
		lua_pushinteger(m_luaState, entity);
		lua_pushnumber(m_luaState, deltaTime);

		if (lua_pcall(m_luaState, 2, 0, 0) != LUA_OK)
			LogLuaError();
	}

	void ScriptSystem::ResetState(SceneGraph* newScene)
	{
		Shutdown();
		Startup();

		if (newScene)
			m_currentScene = newScene;

		m_currentScene->RegisterAllComponents();
	}

	void ScriptSystem::RunInterpreter(void)
	{
		char buff[256];

		while (fgets(buff, sizeof(buff), stdin) != NULL)
		{		
			if (luaL_dostring(m_luaState, buff) != LUA_OK)
				LogLuaError();
		}
	}

	void ScriptSystem::LogLuaError(void)
	{
		std::cout << lua_tostring(m_luaState, -1) << '\n';
		lua_pop(m_luaState, 1);
	}

	void ScriptSystem::RunConfigScript(const char* script)
	{
		std::string path = ScriptSystem::GetConfigScriptsLocation() + script;

		if (luaL_dofile(m_luaState, path.c_str()) != LUA_OK)
			LogLuaError();
	}

}