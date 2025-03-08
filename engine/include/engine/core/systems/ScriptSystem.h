#pragma once

#include <filesystem>

#include "core/TypesECS.h"

namespace engine
{
	class ScriptSystem
	{
	private:

		using FilePath = std::filesystem::path;

	public:

		static void Startup(void);
		static std::string FindConfigScripts(void);

		static void Shutdown(void);

		static void SetCurrentScene(class SceneGraph* graph);

		/*static void ExecuteUpdate(void);
		static void ExecuteFixedUpdate(void);


		static void ExecuteDestroyAll(void);*/

		static SceneGraph*			GetCurrentScene(void);
		static const std::string&	GetConfigScriptsLocation(void);

		static void RegisterNewEntity(EntityHandle newEntity, const std::string& name);

		static void RegisterNewScriptComponent(EntityHandle owner);
		static void RegisterNewScriptObject(const std::string& type, EntityHandle owner);

		static void StartScript(EntityHandle entity);
		static void UpdateScript(EntityHandle entity, float deltaTime);

		static void RunInterpreter(void);

	private:

		static void LogLuaError(void);
		static void RunConfigScript(const char* script);

		static class	SceneGraph* m_currentScene;
		static struct	lua_State* m_luaState;

		static std::string m_configScriptsLocation;
	};
}