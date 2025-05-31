extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "core/SceneGraph.h"
#include "physics/rigidbody/RigidBodyStatic.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/physics/RigidBodyStaticFunctions.h"

int script_GetRigidBodyStaticRef(struct lua_State* luaState)
{
    engine::RigidBodyStatic* rigidBody = nullptr;

    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (handle)");

    else
    {
        engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
        rigidBody =
            engine::ScriptSystem::GetCurrentScene()->GetComponent<engine::RigidBodyStatic>(handle);
    }

    lua_pushlightuserdata(luaState, rigidBody);

    return 1;
}

int script_GetStaticBoxHalfExtents(struct lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyStatic* rigidBody =
        (engine::RigidBodyStatic*)lua_touserdata(luaState, 1))
    {
        lua_pushnumber(luaState, rigidBody->GetBoxHalfExtents().GetX());
        lua_pushnumber(luaState, rigidBody->GetBoxHalfExtents().GetY());
        lua_pushnumber(luaState, rigidBody->GetBoxHalfExtents().GetZ());

        return 3;
    }
    lua_pushnil(luaState);
    return 1;
}

int script_GetStaticSphereRadius(struct lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyStatic* rigidBody =
        (engine::RigidBodyStatic*)lua_touserdata(luaState, 1))
    {
        lua_pushnumber(luaState, rigidBody->GetSphereRadius());

        return 1;
    }
    lua_pushnil(luaState);
    return 1;
}

int script_GetStaticCapsuleFormat(struct lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyStatic* rigidBody =
        (engine::RigidBodyStatic*)lua_touserdata(luaState, 1))
    {
        lua_pushnumber(luaState, rigidBody->GetBoxHalfExtents().GetX());
        lua_pushnumber(luaState, rigidBody->GetBoxHalfExtents().GetY());

        return 2;
    }
    lua_pushnil(luaState);
    return 1;
}

int script_GetStaticIsTrigger(struct lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyStatic* rigidBody =
        (engine::RigidBodyStatic*)lua_touserdata(luaState, 1))
    {
        lua_pushboolean(luaState, rigidBody->GetIsTrigger());
        return 1;
    }
    lua_pushnil(luaState);
    return 1;
}

int script_SetStaticBoxHalfExtents(struct lua_State* luaState)
{
    if (lua_gettop(luaState) != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::RigidBodyStatic* rigidBody =
        (engine::RigidBodyStatic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetBoxHalfExtents(math::Vector3f(static_cast<f32>(lua_tonumber(luaState, 2)),
                                                    static_cast<f32>(lua_tonumber(luaState, 3)),
                                                    static_cast<f32>(lua_tonumber(luaState, 4))));
    }
    return 0;
}

int script_SetStaticSphereRadius(struct lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, radius)");

    else if (engine::RigidBodyStatic* rigidBody =
        (engine::RigidBodyStatic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetSphereRadius(static_cast<f32>(lua_tonumber(luaState, 2)));
    }
    return 0;
}

int script_SetStaticCapsuleFormat(struct lua_State* luaState)
{
    if (lua_gettop(luaState) != 3)
        luaL_error(luaState, "Expected 3 arguments (self, x, y)");

    else if (engine::RigidBodyStatic* rigidBody =
        (engine::RigidBodyStatic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetCapsuleFormat(static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3)));
    }
    return 0;
}

int script_SetStaticTrigger(struct lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, bool)");

    else if (engine::RigidBodyStatic* rigidBody =
        (engine::RigidBodyStatic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetTrigger(static_cast<bool>(lua_toboolean(luaState, 2)));
    }
    return 0;
}

void engine::RegisterRigidBodyStaticFunctions(lua_State* luaState)
{
    constexpr luaL_Reg rigidBodyStaticFuncs[] =
    {
        {"GetRigidBodyStaticRef", script_GetRigidBodyStaticRef},

        {"GetBoxHalfExtents", script_GetStaticBoxHalfExtents},
        {"GetSphereRadius", script_GetStaticSphereRadius},
        {"GetCapsuleFormat", script_GetStaticCapsuleFormat},
        {"GetIsTrigger", script_GetStaticIsTrigger},

        {"SetBoxHalfExtents", script_SetStaticBoxHalfExtents},
        {"SetSphereRadius", script_SetStaticSphereRadius},
        {"SetCapsuleFormat", script_SetStaticCapsuleFormat},
        {"SetTrigger", script_SetStaticTrigger},

        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, rigidBodyStaticFuncs, 0);
    lua_setglobal(luaState, "RigidBodyStaticRef");
}
