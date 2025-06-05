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

#include "scripting/EngineScriptFunctions.h"
#include "utility/Platform.h"



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

        RegisterEngineFunctions();
        RunAllConfigScripts();
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

    void ScriptSystem::UnregisterScriptObject(const std::string& type, EntityHandle owner)
    {
        lua_getglobal(GetInstance()->m_luaState, "_RemoveScriptObject");
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
        std::string luaName;
        
        if (className && (*className != '\0'))
            luaName = className;
        else
            luaName = "NewUserScript";


        std::string fullPath = GetInstance()->m_userScriptsLocation;

        fullPath.push_back('/');

        if (dirRelativePath)
            fullPath.append(dirRelativePath);

        fullPath.append(luaName);
        fullPath.append(".lua");

        if (std::filesystem::exists(fullPath))
            return;

        if (luaName.empty() || luaName.size() >= 64)
            return;

        std::ofstream newFile(fullPath, std::ios::out);

        if (!newFile)
            return;

        const char fileData[] = "%s = ScriptObject:_new()\n\n-- Is executed once when the object becomes \
active\nfunction %s:Start()\n\nend\n\n\n\
-- Is executed every tick\nfunction %s:Update(deltaTime)\n\nend\n\n\n-- Engine definitions\n\
ScriptObjectTypes.%s = %s\nreturn %s";

        char textBuffer[1024];

        sprintf_s(textBuffer, fileData, luaName.c_str(), luaName.c_str(), luaName.c_str(),
                  luaName.c_str(), luaName.c_str(), luaName.c_str());

        newFile << textBuffer;
        newFile.close();
        
        FilePath pathObject(fullPath);
        
        // Open new file in text editor
        OpenFile(std::filesystem::absolute(fullPath).c_str());
        
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

    void ScriptSystem::NotifyCollisionEnter(EntityHandle entityA, EntityHandle entityB)
    {
        if (Entity* entityPtrA = GetInstance()->m_currentScene->GetEntity(entityA))
        {
            if (entityPtrA->HasComponent<Script>())
            {
                lua_getglobal(GetInstance()->m_luaState, "_OnCollisionEnterScript");
                lua_pushinteger(GetInstance()->m_luaState, entityA);
                lua_pushinteger(GetInstance()->m_luaState, entityB);

                if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
                    LogLuaError();
            }
        }

        if (Entity* entityPtrB = GetInstance()->m_currentScene->GetEntity(entityB))
        {
            if (entityPtrB->HasComponent<Script>())
            {
                lua_getglobal(GetInstance()->m_luaState, "_OnCollisionEnterScript");
                lua_pushinteger(GetInstance()->m_luaState, entityB);
                lua_pushinteger(GetInstance()->m_luaState, entityA);

                if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
                    LogLuaError();
            }
        }
    }

    void ScriptSystem::NotifyCollisionExit(EntityHandle entityA, EntityHandle entityB)
    {
        if (Entity* entityPtrA = GetInstance()->m_currentScene->GetEntity(entityA))
        {
            if (entityPtrA->HasComponent<Script>())
            {
                lua_getglobal(GetInstance()->m_luaState, "_OnCollisionExitScript");
                lua_pushinteger(GetInstance()->m_luaState, entityA);
                lua_pushinteger(GetInstance()->m_luaState, entityB);

                if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
                    LogLuaError();
            }
        }

        if (Entity* entityPtrB = GetInstance()->m_currentScene->GetEntity(entityB))
        {
            if (entityPtrB->HasComponent<Script>())
            {
                lua_getglobal(GetInstance()->m_luaState, "_OnCollisionExitScript");
                lua_pushinteger(GetInstance()->m_luaState, entityB);
                lua_pushinteger(GetInstance()->m_luaState, entityA);

                if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
                    LogLuaError();
            }
        }
    }

    void ScriptSystem::NotifyTriggerEnter(EntityHandle entityA, EntityHandle entityB)
    {
        if (Entity* entityPtrA = GetInstance()->m_currentScene->GetEntity(entityA))
        {
            if (entityPtrA->HasComponent<Script>())
            {
                lua_getglobal(GetInstance()->m_luaState, "_OnTriggerEnterScript");
                lua_pushinteger(GetInstance()->m_luaState, entityA);
                lua_pushinteger(GetInstance()->m_luaState, entityB);

                if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
                    LogLuaError();
            }
        }
    }

    void ScriptSystem::NotifyTriggerExit(EntityHandle entityA, EntityHandle entityB)
    {
        if (Entity* entityPtrA = GetInstance()->m_currentScene->GetEntity(entityA))
        {
            if (entityPtrA->HasComponent<Script>())
            {
                lua_getglobal(GetInstance()->m_luaState, "_OnTriggerExitScript");
                lua_pushinteger(GetInstance()->m_luaState, entityA);
                lua_pushinteger(GetInstance()->m_luaState, entityB);

                if (lua_pcall(GetInstance()->m_luaState, 2, 0, 0) != LUA_OK)
                    LogLuaError();
            }
        }
    }

    void ScriptSystem::LogLuaError(void)
    {
        std::cout << lua_tostring(GetInstance()->m_luaState, -1) << '\n';
        lua_pop(GetInstance()->m_luaState, 1);
    }

    void ScriptSystem::RegisterEngineFunctions(void)
    {
        lua_State* state = GetInstance()->m_luaState;

        RegisterResourceFunctions(state);
        RegisterEntityFunctions(state);
        RegisterComponentFunctions(state);
        RegisterScriptComponentFunctions(state);
        RegisterCameraFunctions(state);
        RegisterTransformFunctions(state);
        RegisterInputFunctions(state);
        RegisterUIFunctions(state);
        RegisterUITextFunctions(state);
        RegisterUIButtonFunctions(state);
        RegisterUIProgressBarFunctions(state);
        RegisterVector2Functions(state);
        RegisterVector3Functions(state);
        RegisterRaycastFunctions(state);
        RegisterNavPointFunctions(state);
        RegisterRigidBodyDynamicFunctions(state);
        RegisterRigidBodyStaticFunctions(state);
        RegisterAudioPlayerFunctions(state);
        RegisterRendererFunctions(state);
    }

    void ScriptSystem::RunConfigScript(const char* script)
    {
        std::string path = ScriptSystem::GetConfigScriptsLocation() + script;

        if (luaL_dofile(GetInstance()->m_luaState, path.c_str()) != LUA_OK)
            LogLuaError();
    }

    void ScriptSystem::RunAllConfigScripts(void)
    {
        RunConfigScript("Utils.lua");
        RunConfigScript("ecs/Component.lua");
        RunConfigScript("ecs/Entity.lua");
        RunConfigScript("ScriptObject.lua");
        RunConfigScript("ecs/Script.lua");
        RunConfigScript("ecs/Camera.lua");
        RunConfigScript("ecs/Transform.lua");
        RunConfigScript("Input.lua");
        RunConfigScript("ecs/NavPoint.lua");

        RunConfigScript("ui/Canvas.lua");
        RunConfigScript("ui/Text.lua");
        RunConfigScript("ui/Image.lua");
        RunConfigScript("ui/Button.lua");
        RunConfigScript("ui/ProgressBar.lua");

        RunConfigScript("vector/Vector2.lua");
        RunConfigScript("vector/Vector3.lua");

        RunConfigScript("physics/Raycast.lua");
        RunConfigScript("physics/RigidBodyDynamic.lua");
        RunConfigScript("physics/RigidBodyStatic.lua");

        RunConfigScript("ecs/AudioPlayer.lua");
        RunConfigScript("ecs/Renderer.lua");
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

           /* if (character >= 'A' && character <= 'Z')
                character += 32;*/
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