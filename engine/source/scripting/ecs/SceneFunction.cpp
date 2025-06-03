extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "Engine.h"
#include "core/SceneGraph.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/ecs/SceneFunction.h"

int script_CreateEntity(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount > 2 || argumentCount < 1)
        luaL_error(luaState, "Invalid argument count");

    std::string entityName = lua_tostring(luaState, 1);
    engine::EntityHandle parent = engine::Entity::INVALID_HANDLE;

    if (argumentCount == 2)
        parent = lua_tointeger(luaState, 2);
    
    engine::EntityHandle newEntity =
    engine::ScriptSystem::GetCurrentScene()->CreateEntity(entityName, parent);

    lua_pushinteger(luaState, newEntity);
    engine::Engine::GetEngine()->ResetApplication();
    return 1;
}

int script_DestroyEntity(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (entity)");

    else
    {
        engine::EntityHandle entity = lua_tointeger(luaState, 1);
        engine::ScriptSystem::GetCurrentScene()->DestroyEntity(entity);
        engine::Engine::GetEngine()->ResetApplication();
    }

    return 0;
}

int script_CreateComponent(lua_State* luaState)
{
    using ComponentType = engine::Entity::EComponentFlags;

    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 argument (entity, component type)");

    engine::EntityHandle owner = lua_tointeger(luaState, 1);
    ComponentType type = static_cast<ComponentType>(lua_tointeger(luaState, 2));

    engine::Component* newComponent = nullptr;
    engine::SceneGraph* graph = engine::ScriptSystem::GetCurrentScene();

    switch (type)
    {
    case engine::Entity::TRANSFORM:
        newComponent = graph->CreateComponent<engine::Transform>(owner);
        break;

    case engine::Entity::SCRIPT:
        newComponent = graph->CreateComponent<engine::Script>(owner);
        break;

    case engine::Entity::RENDERER:
        newComponent = graph->CreateComponent<engine::Renderer>(owner);
        break;

    case engine::Entity::RIGIDBODY_DYNAMIC:
        newComponent = graph->CreateComponent<engine::RigidBodyDynamic>(owner);
        break;

    case engine::Entity::RIGIDBODY_STATIC:
        newComponent = graph->CreateComponent<engine::RigidBodyStatic>(owner);
        break;

    case engine::Entity::CAMERA:
        newComponent = graph->CreateComponent<engine::Camera>(owner);
        break;

    case engine::Entity::AUDIO_PLAYER:
        newComponent = graph->CreateComponent<engine::AudioPlayer>(owner);
        break;

    case engine::Entity::TRIANGLE_MESH:
        newComponent = graph->CreateComponent<engine::TriangleMesh>(owner);
        break;

    case engine::Entity::NAVIGATION_POINT:
        newComponent = graph->CreateComponent<engine::NavigationPoint>(owner);
        break;

    case engine::Entity::LIGHT_SOURCE:
        newComponent = graph->CreateComponent<engine::LightSource>(owner);
        break;

    default:
        break;
    }

    lua_pushlightuserdata(luaState, newComponent);
    engine::Engine::GetEngine()->ResetApplication();
    return 1;
}

int script_DestroyComponent(lua_State* luaState)
{
    using ComponentType = engine::Entity::EComponentFlags;

    engine::EntityHandle owner = lua_tointeger(luaState, 1);
    ComponentType type = static_cast<ComponentType>(lua_tointeger(luaState, 2));

    engine::SceneGraph* graph = engine::ScriptSystem::GetCurrentScene();

    switch (type)
    {
    case engine::Entity::TRANSFORM:
        graph->DestroyComponent<engine::Transform>(owner);
        break;

    case engine::Entity::SCRIPT:
        graph->DestroyComponent<engine::Script>(owner);
        break;

    case engine::Entity::RENDERER:
        graph->DestroyComponent<engine::Renderer>(owner);
        break;

    case engine::Entity::RIGIDBODY_DYNAMIC:
        graph->DestroyComponent<engine::RigidBodyDynamic>(owner);
        break;

    case engine::Entity::RIGIDBODY_STATIC:
        graph->DestroyComponent<engine::RigidBodyStatic>(owner);
        break;

    case engine::Entity::CAMERA:
        graph->DestroyComponent<engine::Camera>(owner);
        break;

    case engine::Entity::AUDIO_PLAYER:
        graph->DestroyComponent<engine::AudioPlayer>(owner);
        break;

    case engine::Entity::TRIANGLE_MESH:
        graph->DestroyComponent<engine::TriangleMesh>(owner);
        break;

    case engine::Entity::NAVIGATION_POINT:
        graph->DestroyComponent<engine::NavigationPoint>(owner);
        break;

    case engine::Entity::LIGHT_SOURCE:
        graph->DestroyComponent<engine::LightSource>(owner);
        break;

    default:
        break;
    }

    engine::Engine::GetEngine()->ResetApplication();
    return 0;
}

namespace engine
{
    void RegisterSceneFunctions(lua_State* luaState)
    {
        constexpr luaL_Reg sceneFuncs[]
        {
            {"CreateEntity", script_CreateEntity},
            {"DestroyEntity", script_DestroyEntity},
            {"CreateComponent", script_CreateComponent},
            {"DestroyComponent", script_DestroyComponent},
            {NULL, NULL}
        };

        lua_newtable(luaState);
        luaL_setfuncs(luaState, sceneFuncs, 0);
        lua_setglobal(luaState, "Scene");
    }
}