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
#include "scripting/CameraFunctions.h"
#include "scripting/TransformFunctions.h"
#include "scripting/InputFunctions.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>


namespace engine
{
    ScriptSystem* ScriptSystem::m_instance = nullptr;
    std::mutex  ScriptSystem::m_mutex;
    
    void ScriptSystem::Startup(void)
    {
        GetInstance()->m_luaState = luaL_newstate();

        if (!GetInstance()->m_luaState)
        {
            // TODO: log error?
            std::cout << "Unable to create Lua state\n";
            exit(EXIT_FAILURE);			
        }

        luaL_openlibs(GetInstance()->m_luaState);

        RegisterEntityFunctions(GetInstance()->m_luaState);
        RegisterComponentFunctions(GetInstance()->m_luaState);
        RegisterScriptComponentFunctions(GetInstance()->m_luaState);
        RegisterCameraFunctions(GetInstance()->m_luaState);
        RegisterTransformFunctions(GetInstance()->m_luaState);
        RegisterInputFunctions(GetInstance()->m_luaState);

        RunConfigScript("Component.lua");
        RunConfigScript("Entity.lua");

        RunConfigScript("ScriptObject.lua");
        RunConfigScript("Script.lua");

        RunConfigScript("Camera.lua");
        RunConfigScript("Transform.lua");

        RunConfigScript("Input.lua");

        RunAllUserScripts();
    }

    void ScriptSystem::Shutdown(bool deleteInstance)
    {
        lua_close(GetInstance()->m_luaState);
        
        if (deleteInstance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    void ScriptSystem::SetCurrentScene(SceneGraph* graph)
    {
        GetInstance()->m_currentScene = graph;
    }

    SceneGraph* ScriptSystem::GetCurrentScene(void)
    {
        return GetInstance()->m_currentScene;
    }

    const std::string& ScriptSystem::GetConfigScriptsLocation(void)
    {
        return GetInstance()->m_configScriptsLocation;
    }

    void ScriptSystem::RegisterNewComponent(const char* function, EntityHandle owner)
    {
        lua_getglobal(GetInstance()->m_luaState, function);
        lua_pushinteger(GetInstance()->m_luaState, owner);

        if (lua_pcall(GetInstance()->m_luaState, 1, 0, 0) != LUA_OK)
            LogLuaError();
    }

    void ScriptSystem::UnregisterComponent(const char* function, EntityHandle owner)
    {
        lua_getglobal(GetInstance()->m_luaState, function);
        lua_pushinteger(GetInstance()->m_luaState, owner);

        if (lua_pcall(GetInstance()->m_luaState, 1, 0, 0) != LUA_OK)
            LogLuaError();
    }

    void ScriptSystem::RegisterNewEntity(EntityHandle newEntity, const std::string& name)
    {
        if (name.empty())
            return;

        lua_getglobal(GetInstance()->m_luaState, "_AddEntityHandle");
        lua_pushinteger(GetInstance()->m_luaState, newEntity);
        lua_pushstring(GetInstance()->m_luaState, name.c_str());

        if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
            LogLuaError();
    }

    void ScriptSystem::UnregisterEntity(EntityHandle toRemove)
    {
        std::string fullName = GetInstance()->m_currentScene->GetFullEntityName(toRemove);

        lua_getglobal(GetInstance()->m_luaState, "_UnregisterEntity");
        lua_pushstring(GetInstance()->m_luaState, fullName.c_str());

        if (lua_pcall(GetInstance()->m_luaState, 1, 0, 0) != LUA_OK)
            LogLuaError();
    }

    void ScriptSystem::RegisterNewScriptObject(const std::string& type, EntityHandle owner)
    {
        lua_getglobal(GetInstance()->m_luaState, "_NewScriptObject");
        lua_pushstring(GetInstance()->m_luaState, type.c_str());
        lua_pushinteger(GetInstance()->m_luaState, owner);

        if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
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
            return currentPath.string();

        // default to sibling folder
        else
            return parentPath.append("scripts\\").string();
    }

    void ScriptSystem::StartScript(EntityHandle entity)
    {
        lua_getglobal(GetInstance()->m_luaState, "_StartScript");
        lua_pushinteger(GetInstance()->m_luaState, entity);

        if (lua_pcall(GetInstance()->m_luaState, 1, 0, 0) != LUA_OK)
            LogLuaError();
    }

    void ScriptSystem::UpdateScript(EntityHandle entity, f32 deltaTime)
    {
        lua_getglobal(GetInstance()->m_luaState, "_UpdateScript");
        lua_pushinteger(GetInstance()->m_luaState, entity);
        lua_pushnumber(GetInstance()->m_luaState, deltaTime);

        if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
            LogLuaError();
    }

    void ScriptSystem::ResetState(SceneGraph* newScene)
    {
        Shutdown(false);
        Startup();

        if (newScene)
        {
            GetInstance()->m_currentScene = newScene;
            GetInstance()->m_currentScene->RegisterAllEntities();
            GetInstance()->m_currentScene->RegisterAllComponents();
        }
    }

    void ScriptSystem::SetUserScriptLocation(const char* path)
    {
        GetInstance()->m_userScriptsLocation = path;
    }

    void ScriptSystem::CreateUserScript(const char* dirRelativePath, const char* className)
    {
        std::string fullPath = GetInstance()->m_userScriptsLocation +
                              ((dirRelativePath) ? dirRelativePath : "") +
                              ((className) ? className : "UserScript") + ".lua";

        if (std::filesystem::exists(fullPath))
            return;

        std::string formattedName = FormatLuaClassName(className);

        if (formattedName.empty() || formattedName.size() >= 64)
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
            if (luaL_dostring(GetInstance()->m_luaState, buff) != LUA_OK)
                LogLuaError();
        }
    }

    void ScriptSystem::LogLuaError(void)
    {
        std::cout << lua_tostring(GetInstance()->m_luaState, -1) << '\n';
        lua_pop(GetInstance()->m_luaState, 1);
    }

    void ScriptSystem::RunConfigScript(const char* script)
    {
        std::string path = ScriptSystem::GetConfigScriptsLocation() + script;

        if (luaL_dofile(GetInstance()->m_luaState, path.c_str()) != LUA_OK)
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

    ScriptSystem* ScriptSystem::GetInstance(void)
    {
        if (!m_instance)
        {
            m_instance = new ScriptSystem();
        }

        return m_instance;
    }

    void ScriptSystem::RunUserScript(const std::string& filename)
    {
        if (luaL_dofile(GetInstance()->m_luaState, filename.c_str()) != LUA_OK)
            LogLuaError();
    }

    void ScriptSystem::RunAllUserScripts(void)
    {	
        if (!std::filesystem::exists(GetInstance()->m_userScriptsLocation))
            return;

        for (auto& file : std::filesystem::recursive_directory_iterator(GetInstance()->m_userScriptsLocation))
        {
            if (!file.exists())
                continue;

            if (!file.is_regular_file())
                continue;

            std::string filename = file.path().string();
            uint64 filenameLength = filename.size();

            // look for ".lua" extension ans skip if not found
            if (filenameLength <= 4 || memcmp(filename.data() + filenameLength - 4, ".lua", 4))
                continue;

            std::cout << "Running file: " << filename << '\n';
            RunUserScript(filename);
        }	
    }


}