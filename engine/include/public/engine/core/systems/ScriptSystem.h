#pragma once

#include <filesystem>
#include <mutex>

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

        // Script-related function other than SetUserScriptLocation().
        // SetUserScriptLocation should be called before this function on engine startup.
        ENGINE_API
        static void Startup(void);

        // Find where the engine's internal scripts
        // are stored
        ENGINE_API
        static std::string FindConfigScripts(void);
        
        // Close lua state. Do not call any script-related
        // functions after this
        ENGINE_API
        static void Shutdown(bool deleteInstance = true);

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

        // Register a component of a given type
        // function: lua global function that creates a table for an existing component
        // owner: owning entity 
        ENGINE_API
        static void RegisterNewComponent(const char* function, EntityHandle owner);

        // Remove an existing component
        // function: lua global function that handles the component's deletion
        // owner: owning entity 
        ENGINE_API
        static void UnregisterComponent(const char* function, EntityHandle owner);

        // Add an entity to the scripting system's lua entity table
        ENGINE_API
        static void RegisterNewEntity(EntityHandle newEntity, const std::string& name);

        // Remove an entity from the system's entity table
        ENGINE_API
        static void UnregisterEntity(EntityHandle toRemove);

        // Add a script object to an existing script component in script
        ENGINE_API
        static void RegisterNewScriptObject(const std::string& type, EntityHandle owner);

        // Call all Start() functions in a given entity's script component
        ENGINE_API
        static void StartScript(EntityHandle entity);

        // Call all Update() functions in a given entity's script component
        ENGINE_API
        static void UpdateScript(EntityHandle entity, f32 deltaTime);

        // Fully shutdown and restart lua state, reset all existing lua
        // objecs and re run all scripts
        ENGINE_API
        static void ResetState(class SceneGraph* newScene = nullptr);

        // Set where the default user-generated scripts are stored for this project
        ENGINE_API
        static void SetUserScriptLocation(const char* path);

        // Create a new .lua file inheriting from ScriptObject
        ENGINE_API
        static void CreateUserScript(const char* dirRelativePath, const char* className);

        // Run all .lua files present in the directory specified as
        // user script locations
        ENGINE_API
        static void RunAllUserScripts(void);

        // Run a debug lua interpreter
        // NOTE: there is no way to stop it beside killing the
        // entire program as of now
        // TODO: add kill condition once we have the input system ready
        ENGINE_API
        static void RunInterpreter(void);


        ENGINE_API
        static void NotifyCollisionEnter(EntityHandle entityA, EntityHandle entityB);

        ENGINE_API
        static void NotifyCollisionExit(EntityHandle entityA, EntityHandle entityB);

    private:

        static void RunUserScript(const std::string& filename);
        static void LogLuaError(void);

        // Run an internal lua file
        static void RunConfigScript(const char* script);

        static std::string FormatLuaClassName(const char* name);

        static ScriptSystem* GetInstance(void);


        class	SceneGraph* m_currentScene = nullptr;
        struct	lua_State*	m_luaState = nullptr;

        std::string			m_configScriptsLocation = ScriptSystem::FindConfigScripts();
        std::string			m_userScriptsLocation;

        static std::mutex	 m_mutex;
        static ScriptSystem* m_instance;
    };
}