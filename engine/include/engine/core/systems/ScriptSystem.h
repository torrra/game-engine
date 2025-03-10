#pragma once

#include <filesystem>

#include "engine/EngineExport.h"
#include "engine/core/TypesECS.h"

namespace engine
{
	class ScriptSystem
	{
	private:

		using FilePath = std::filesystem::path;

	public:

		// Initialize lua state, register C functions and
		// run engine scripts. Should be called before any
		// Script-related function
		ENGINE_API
		static void Startup(void);

		// Find where the engine's internal scripts
		// are stored
		ENGINE_API
		static std::string FindConfigScripts(void);
		
		// Close lua state. Do not call any script-related
		// functions after this
		ENGINE_API
		static void Shutdown(void);

		// Set the scripting system's active scene. This function needs to be
		// called every time a scene is loaded, so that the scripting system
		// interacts with the right scene graph
		ENGINE_API
		static void SetCurrentScene(class SceneGraph* graph);


		// Get the current scene graph the scripting system is operating on
		ENGINE_API
		static SceneGraph*			GetCurrentScene(void);

		// Get the engine's scripts location as a string
		ENGINE_API
		static const std::string&	GetConfigScriptsLocation(void);


		// Add an entity to the scripting system's lua entity table
		ENGINE_API
		static void RegisterNewEntity(EntityHandle newEntity, const std::string& name);

		// Add an entity's script component to the scripting system's lua script component table
		ENGINE_API
		static void RegisterNewScriptComponent(EntityHandle owner);

		// Add a script object to an existing script component in script
		ENGINE_API
		static void RegisterNewScriptObject(const std::string& type, EntityHandle owner);

		// Call all Start() functions in a given entity's script component
		ENGINE_API
		static void StartScript(EntityHandle entity);

		// Call all Update() functions in a given entity's script component
		ENGINE_API
		static void UpdateScript(EntityHandle entity, float deltaTime);

		// Fully shutdown and restart lua state, reset all existing lua
		// objecs and re run all scripts
		static void ResetState(class SceneGraph* newScene = nullptr);

		// Run a debug lua interpreter
		// NOTE: there is no way to stop it beside killing the
		// entire program as of now
		// TODO: add kill condition once we have the input system ready
		ENGINE_API
		static void RunInterpreter(void);

	private:

		static void LogLuaError(void);

		// Run an internal lua file
		static void RunConfigScript(const char* script);

		static class	SceneGraph* m_currentScene;
		static struct	lua_State* m_luaState;

		static std::string m_configScriptsLocation;
	};
}