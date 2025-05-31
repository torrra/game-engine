extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "core/SceneGraph.h"
#include "physics/rigidbody/RigidBodyDynamic.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/physics/RigidBodyDynamicFunctions.h"

int script_GetRigidBodyDynamicRef(lua_State* luaState)
{
    engine::RigidBodyDynamic* rigidBody = nullptr;

    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (handle)");

    else
    {
        engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
        rigidBody = 
            engine::ScriptSystem::GetCurrentScene()->GetComponent<engine::RigidBodyDynamic>(handle);
    }

    lua_pushlightuserdata(luaState, rigidBody);

    return 1;
}

int script_GetIsGravityDisabled(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {

        lua_pushboolean(luaState, rigidBody->IsGravityDisabled());
        return 1;
    }

    lua_pushnil(luaState);
    return 0;
}

int script_GetBoxHalfExtents(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushnumber(luaState, rigidBody->GetBoxHalfExtents().GetX());
        lua_pushnumber(luaState, rigidBody->GetBoxHalfExtents().GetY());
        lua_pushnumber(luaState, rigidBody->GetBoxHalfExtents().GetZ());

        return 3;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_GetSphereRadius(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushnumber(luaState, rigidBody->GetSphereRadius());

        return 1;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_GetCapsuleFormat(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushnumber(luaState, rigidBody->GetCapsuleFormat().GetX());
        lua_pushnumber(luaState, rigidBody->GetCapsuleFormat().GetY());

        return 2;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_GetLinearVelocity(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushnumber(luaState, rigidBody->GetLinearVelocity().GetX());
        lua_pushnumber(luaState, rigidBody->GetLinearVelocity().GetY());
        lua_pushnumber(luaState, rigidBody->GetLinearVelocity().GetZ());

        return 3;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_GetAngularVelocity(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushnumber(luaState, rigidBody->GetAngularVelocity().GetX());
        lua_pushnumber(luaState, rigidBody->GetAngularVelocity().GetY());
        lua_pushnumber(luaState, rigidBody->GetAngularVelocity().GetZ());

        return 3;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_GetIsTrigger(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushboolean(luaState, rigidBody->GetIsTrigger());
        return 1;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_GetIsXAxisLock(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushboolean(luaState, rigidBody->GetIsXAxisLock());
        return 1;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_GetIsYAxisLock(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushboolean(luaState, rigidBody->GetIsYAxisLock());
        return 1;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_GetIsZAxisLock(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        lua_pushboolean(luaState, rigidBody->GetIsZAxisLock());
        return 1;
    }
    lua_pushnil(luaState);
    return 0;
}

int script_SetGravityDisabled(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, bool)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetGravityDisabled(static_cast<bool>(lua_toboolean(luaState, 2)));
    }

    return 0;
}

int script_SetBoxHalfExtents(lua_State* luaState)
{
    if (lua_gettop(luaState) != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetBoxHalfExtents(math::Vector3f(static_cast<f32>(lua_tonumber(luaState, 2)),
                                                    static_cast<f32>(lua_tonumber(luaState, 3)),
                                                    static_cast<f32>(lua_tonumber(luaState, 4))));
    }
    return 0;
}

int script_SetSphereRadius(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, radius)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetSphereRadius(static_cast<f32>(lua_tonumber(luaState, 2)));
    }
    return 0;
}

int script_SetCapsuleFormat(lua_State* luaState)
{
    if (lua_gettop(luaState) != 3)
        luaL_error(luaState, "Expected 3 arguments (self, x, y)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetCapsuleFormat(static_cast<f32>(lua_tonumber(luaState, 2)),
                                    static_cast<f32>(lua_tonumber(luaState, 3)));
    }
    return 0;
}

int script_SetTrigger(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, bool)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetTrigger(static_cast<bool>(lua_toboolean(luaState, 2)));
    }
    return 0;
}

int script_SetLinearVelocity(lua_State* luaState)
{
    if (lua_gettop(luaState) != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetLinearVelocity(math::Vector3f(
                                        static_cast<f32>(lua_tonumber(luaState, 2)),
                                        static_cast<f32>(lua_tonumber(luaState, 3)),
                                        static_cast<f32>(lua_tonumber(luaState, 4))));
    }
    return 0;
}

int script_SetAngularVelocity(lua_State* luaState)
{
    if (lua_gettop(luaState) != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetAngularVelocity(math::Vector3f(
                                        static_cast<f32>(lua_tonumber(luaState, 2)),
                                        static_cast<f32>(lua_tonumber(luaState, 3)),
                                        static_cast<f32>(lua_tonumber(luaState, 4))));
    }
    return 0;
}

int script_SetXAxisLock(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, bool)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetXAxisLock(static_cast<bool>(lua_toboolean(luaState, 2)));
    }
    return 0;
}

int script_SetYAxisLock(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, bool)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetYAxisLock(static_cast<bool>(lua_toboolean(luaState, 2)));
    }
    return 0;
}

int script_SetZAxisLock(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, bool)");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        rigidBody->SetZAxisLock(static_cast<bool>(lua_toboolean(luaState, 2)));
    }
    return 0;
}

int script_AddForce(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);
    if (argumentCount < 4 || argumentCount > 6)
        luaL_error(
            luaState, "Expected 4 or 5 or 6 arguments (self, x, y, z [, force mode, auto wake]");

    else if (engine::RigidBodyDynamic* rigidBody =
             (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        uint32  forceMode   = 0;
        bool    autoWake    = true;

        if (argumentCount == 5)
            forceMode       = rigidBody->SetForceMode(static_cast<uint32>(lua_tointeger(luaState, 5)));

        if (argumentCount == 6)
        {
            forceMode       = rigidBody->SetForceMode(static_cast<uint32>(lua_tointeger(luaState, 5)));
            autoWake        = static_cast<bool>(lua_toboolean(luaState, 6));
        }

        rigidBody->AddForce(math::Vector3f(
                                static_cast<f32>(lua_tonumber(luaState, 2)),
                                static_cast<f32>(lua_tonumber(luaState, 3)),
                                static_cast<f32>(lua_tonumber(luaState, 4))),
                                static_cast<engine::EForceMode>(forceMode), autoWake);
    }
    return 0;
}

int script_AddTorque(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);
    if (argumentCount < 4 || argumentCount > 6)
        luaL_error(
            luaState, "Expected 4 or 5 or 6 arguments (self, x, y, z [, force mode, auto wake]");

    else if (engine::RigidBodyDynamic* rigidBody =
        (engine::RigidBodyDynamic*)lua_touserdata(luaState, 1))
    {
        uint32  forceMode   = 0;
        bool    autoWake    = true;

        if (argumentCount == 5)
            forceMode       = rigidBody->SetForceMode(static_cast<uint32>(lua_tointeger(luaState, 5)));

        if (argumentCount == 6)
        {
            forceMode       = rigidBody->SetForceMode(static_cast<uint32>(lua_tointeger(luaState, 5)));
            autoWake        = static_cast<bool>(lua_toboolean(luaState, 6));
        }

        rigidBody->AddTorque(math::Vector3f(
                                static_cast<f32>(lua_tonumber(luaState, 2)),
                                static_cast<f32>(lua_tonumber(luaState, 3)),
                                static_cast<f32>(lua_tonumber(luaState, 4))),
                                static_cast<engine::EForceMode>(forceMode), autoWake);
    }
    return 0;
}

void engine::RegisterRigidBodyDynamicFunctions(lua_State* luaState)
{
    constexpr luaL_Reg rigidBodyDynamicFuncs[] =
    {
        {"GetRigidBodyDynamicRef", script_GetRigidBodyDynamicRef},

        {"GetIsGravityDisabled", script_GetIsGravityDisabled},
        {"GetBoxHalfExtents", script_GetBoxHalfExtents},
        {"GetSphereRadius", script_GetSphereRadius},
        {"GetCapsuleFormat", script_GetCapsuleFormat},
        {"GetLinearVelocity", script_GetLinearVelocity},
        {"GetAngularVelocity", script_GetAngularVelocity},
        {"GetIsTrigger", script_GetIsTrigger},
        {"GetIsXAxisLock", script_GetIsXAxisLock},
        {"GetIsYAxisLock", script_GetIsYAxisLock},
        {"GetIsZAxisLock", script_GetIsZAxisLock},

        {"SetGravityDisabled", script_SetGravityDisabled},
        {"SetBoxHalfExtents", script_SetBoxHalfExtents},
        {"SetSphereRadius", script_SetSphereRadius},
        {"SetCapsuleFormat", script_SetCapsuleFormat},
        {"SetTrigger", script_SetTrigger},
        {"SetLinearVelocity", script_SetLinearVelocity},
        {"SetAngularVelocity", script_SetAngularVelocity},
        {"SetXAxisLock", script_SetXAxisLock},
        {"SetYAxisLock", script_SetYAxisLock},
        {"SetZAxisLock", script_SetZAxisLock},

        {"AddForce", script_AddForce},
        {"AddTorque", script_AddTorque},

        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, rigidBodyDynamicFuncs, 0);
    lua_setglobal(luaState, "RigidBodyDynamicRef");
}
