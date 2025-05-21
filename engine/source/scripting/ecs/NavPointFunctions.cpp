extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "core/SceneGraph.h"
#include "core/components/NavigationPoint.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/ecs/NavPointFunctions.h"

int script_GetNavPointRef(lua_State* luaState)
{
    engine::NavigationPoint* navPoint = nullptr;

    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (handle)");

    else
    {
        engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
        navPoint = engine::ScriptSystem::GetCurrentScene()->GetComponent<engine::NavigationPoint>(handle);
    }

    lua_pushlightuserdata(luaState, navPoint);
    return 1;
}

int script_GetNavPointPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::NavigationPoint* navPoint = (engine::NavigationPoint*)lua_touserdata(luaState, 1))
    {
        math::Vector3f pos = navPoint->GetPosition();

        lua_pushnumber(luaState, static_cast<lua_Number>(pos.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(pos.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(pos.GetZ()));
    }

    return 3;
}

void engine::RegisterNavPointFunctions(lua_State* luaState)
{
    constexpr luaL_Reg navPointFuncs[] =
    {
        {"GetNavPointRef", script_GetNavPointRef},

        {"GetNavPointPosition", script_GetNavPointPosition},

        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, navPointFuncs, 0);
    lua_setglobal(luaState, "NavPointRef");
}
