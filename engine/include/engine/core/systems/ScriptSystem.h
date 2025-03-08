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
		static void Shutdown(void);

		static void SetCurrentScene(class SceneGraph* graph);

		/*static void ExecuteUpdate(void);
		static void ExecuteFixedUpdate(void);


		static void ExecuteDestroyAll(void);*/

		static SceneGraph* GetCurrentScene(void);
		static const std::string& GetConfigScriptsLocation(void);

		static void RegisterNewEntity(EntityHandle newEntity, const std::string& name);
		static std::string FindConfigScripts(void);

		static void RunInterpreter(void);


	private:


		static class	SceneGraph* m_currentScene;
		static struct	lua_State* m_luaState;

		static std::string m_configScriptsLocation;
	};
}