extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include <iostream>
#include <fstream>
#include <iostream>

#include "core/components/Script.h"
#include "core/systems/ScriptSystem.h"
#include "core/SceneGraph.h"

#include "scripting/EntityScriptFunctions.h"
#include "scripting/ComponentFunctions.h"
#include "scripting/ScriptFunctions.h"

//#include "utility/StringConversion.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>


namespace engine
{
	SceneGraph* ScriptSystem::m_currentScene = nullptr;
	lua_State*  ScriptSystem::m_luaState = nullptr;

	// TODO: get rid of hardcoded path
	std::string ScriptSystem::m_configScriptsLocation = ScriptSystem::FindConfigScripts();
	std::string ScriptSystem::m_userScriptsLocation = "./";
	
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

		RunAllUserScripts();
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

	void ScriptSystem::SetUserScriptLocation(const char* path)
	{
		m_userScriptsLocation = path;
	}

	void ScriptSystem::CreateUserScript(const char* dirRelativePath, const char* className)
	{
		std::string fullPath = m_userScriptsLocation +
							  ((dirRelativePath) ? dirRelativePath : "") +
							  ((className) ? className : "UserScript") + ".lua";

		if (std::filesystem::exists(fullPath))
			return;

		std::string formattedName = FormatLuaClassName(className);

		if (formattedName.empty())
			return;

		std::ofstream newFile(fullPath,
							 std::ios::out);

		if (!newFile)
			return;

		const char fileData[] = "%s = ScriptObject:_new()\n\n-- Define member variables \
here\n\n-- Is executed once when the object becomes active\nfunction %s:Start()\n\nend\n\n\n\
-- Is executed every tick\nfunction %s:Update(deltaTime)\n\nend\n\n\n-- Engine definitions\n\
ScriptObjectTypes.%s = %s\nreturn %s";

		char textBuffer[1024];

		sprintf_s(textBuffer, fileData, className, className, className,
									  formattedName.c_str(), className, className);

		newFile << textBuffer;
		newFile.close();
		
		FilePath pathObject(fullPath);
		
		// Open new file in text editor
		ShellExecuteA(NULL, NULL, std::filesystem::absolute(fullPath).string().c_str(), NULL, NULL, SW_SHOW);

		// Run lua file to register existing type
		RunUserScript(fullPath);
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

	std::string ScriptSystem::FormatLuaClassName(const char* name)
	{
		constexpr uint64 maxClassNameLength = 64;
		std::string formattedName = name;


		if (formattedName.size() > maxClassNameLength)
			return std::string();

		for (char& character : formattedName)
		{
			if ((!isalnum(character)) && (character != '_'))
				return std::string();

			if (character >= 'A' && character <= 'Z')
				character += 32;
		}

		return formattedName;
	}

	void ScriptSystem::RunUserScript(const std::string& filename)
	{
		if (luaL_dofile(m_luaState, filename.c_str()) != LUA_OK)
			LogLuaError();
	}

	void ScriptSystem::RunAllUserScripts(void)
	{	
		for (auto& file : std::filesystem::recursive_directory_iterator(m_userScriptsLocation))
		{
			if (!file.exists())
				continue;

			if (!file.is_regular_file())
				continue;

			std::string filename = file.path().string();

			if (filename.find(".lua") == std::string::npos)
				continue;

			std::cout << "Running file: " << filename << '\n';
			RunUserScript(filename);
		}	
	}


}