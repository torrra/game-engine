#include "scripting/physics/RaycastFunctions.h"
#include "physics/Raycast.h"

extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

int script_GetRaycastOrigin(lua_State* luaState)
{

    if (lua_gettop(luaState) != 1)
    {
        luaL_error(luaState, "Expected 1 argument (index)");
        lua_pushnil(luaState);
        return 1;
    }

     int32 index = static_cast<int32>(luaL_checkinteger(luaState, 1));

    if (engine::Raycast* ray = engine::Raycast::GetRay(index))
    {
        math::Vector3f origin = ray->GetOrigin();

        lua_pushnumber(luaState, origin.GetX());
        lua_pushnumber(luaState, origin.GetY());
        lua_pushnumber(luaState, origin.GetZ());
        return 3;
    }
    else
    {
        lua_pushnil(luaState);
        return 1;
    }

}

int script_GetRaycastDirection(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
    {
        luaL_error(luaState, "Expected 1 argument (index)");
        lua_pushnil(luaState);
        return 1;
    }

     int32 index = static_cast<int32>(luaL_checkinteger(luaState, 1));

    if (engine::Raycast* ray = engine::Raycast::GetRay(index))
    {
        math::Vector3f direction = ray->GetDirection();

        lua_pushnumber(luaState, direction.GetX());
        lua_pushnumber(luaState, direction.GetY());
        lua_pushnumber(luaState, direction.GetZ());
        return 3;
    }
    else
    {
        luaL_error(luaState, "Invalid raycast");
        lua_pushnil(luaState);
        return 1;
    }

}

int script_GetRaycastDistance(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
    {
        luaL_error(luaState, "Expected 1 argument (index)");
        lua_pushnil(luaState);
        return 1;
    }

     int32 index = static_cast<int32>(luaL_checkinteger(luaState, 1));

    if (engine::Raycast* ray = engine::Raycast::GetRay(index))
    {
        lua_pushnumber(luaState, ray->GetDistance());
        return 1;
    }
    else
    {
        luaL_error(luaState, "Invalid raycast");
        lua_pushnil(luaState);
        return 1;
    }
}

int script_SetRaycastOrigin(lua_State* luaState)
{
    if (lua_gettop(luaState) != 4)
    {
        luaL_error(luaState, "Expected 4 arguments (index, x, y, z)");
        return 0;
    }

     int32 index = static_cast<int32>(luaL_checkinteger(luaState, 1));

    if (engine::Raycast* ray = engine::Raycast::GetRay(index))
    {
       math::Vector3f origin
       {
           static_cast<f32>(lua_tonumber(luaState, 2)),
           static_cast<f32>(lua_tonumber(luaState, 3)),
           static_cast<f32>(lua_tonumber(luaState, 4))
       };

       ray->SetOrigin(origin);

    }
    else
        luaL_error(luaState, "Invalid raycast");

    return 0;
}

int script_SetRaycastDirection(lua_State* luaState)
{
    if (lua_gettop(luaState) != 4)
    {
        luaL_error(luaState, "Expected 4 arguments (index, x, y, z)");
        return 0;
    }

     int32 index = static_cast<int32>(luaL_checkinteger(luaState, 1));

    if (engine::Raycast* ray = engine::Raycast::GetRay(index))
    {
        math::Vector3f direction
        {
            static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3)),
            static_cast<f32>(lua_tonumber(luaState, 4))
        };

        direction.Normalize();
        ray->SetDirection(direction);

    }
    else
        luaL_error(luaState, "Invalid raycast");

    return 0;
}

int script_SetRaycastDistance(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
    {
        luaL_error(luaState, "Expected 2 arguments (index, distance)");
        return 0;
    }

     int32 index = static_cast<int32>(luaL_checkinteger(luaState, 1));

    if (engine::Raycast* ray = engine::Raycast::GetRay(index))
        ray->SetDistance(static_cast<f32>(lua_tonumber(luaState, 2)));

    else
        luaL_error(luaState, "Invalid raycast");

    return 0;
}


int script_HasRaycastHit(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (index)");

    int32 index = static_cast<int32>(luaL_checkinteger(luaState, 1));
    engine::Raycast::HitData hit;

    if (engine::Raycast* ray = engine::Raycast::GetRay(index))
        lua_pushboolean(luaState, ray->HasHit(&hit));

    else
        luaL_error(luaState, "Invalid raycast");

    lua_pushinteger(luaState, hit.m_hitEntity);

    lua_pushnumber(luaState, hit.m_position.GetX());
    lua_pushnumber(luaState, hit.m_position.GetY());
    lua_pushnumber(luaState, hit.m_position.GetZ());

    lua_pushnumber(luaState, hit.m_distance);
    return 6;   
}

int script_CreateRaycast(lua_State* luaState)
{
    int32 ray = engine::Raycast::CreateRaycast(math::Vector3f(0.f), math::Vector3f(0.f), 10.f);

    lua_pushinteger(luaState, ray);
    return 1;
}

void engine::RegisterRaycastFunctions(lua_State* luaState)
{
    constexpr luaL_Reg raycastFuncs[]
    {
        {"GetOrigin", script_GetRaycastOrigin},
        {"GetDirection", script_GetRaycastDirection},
        {"GetDistance", script_GetRaycastDistance},

        {"SetOrigin", script_SetRaycastOrigin},
        {"SetDirection", script_SetRaycastDirection},
        {"SetDistance", script_SetRaycastDistance},

        {"HasHit", script_HasRaycastHit},
        {"Create", script_CreateRaycast},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, raycastFuncs, 0);
    lua_setglobal(luaState, "RayC");
}
