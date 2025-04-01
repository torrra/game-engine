extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include <iostream>
#include <fstream>
#include <iostream>

#include "input/Input.h"

#include "core/components/Script.h"
#include "core/systems/ScriptSystem.h"
#include "core/SceneGraph.h"

#include "scripting/InputFunctions.h"

static int GetLuaArgKey(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (keyCode)");

    return static_cast<int32>(lua_tointeger(luaState, 1));
}


int script_IsInputPressed(lua_State* luaState)
{
    bool result = engine::Input::IsInputPressed(GetLuaArgKey(luaState));

    lua_pushboolean(luaState, result);
    return 1;
}

int script_IsInputDown(lua_State* luaState)
{
    bool result = engine::Input::IsInputDown(GetLuaArgKey(luaState));

    lua_pushboolean(luaState, result);
    return 1;
}

int script_IsInputHeld(lua_State* luaState)
{
    bool result = engine::Input::IsInputHeld(GetLuaArgKey(luaState));

    lua_pushboolean(luaState, result);
    return 1;
}

int script_IsInputReleased(lua_State* luaState)
{
    bool result = engine::Input::IsInputReleased(GetLuaArgKey(luaState));

    lua_pushboolean(luaState, result);
    return 1;
}

int script_GetCursorPosition(lua_State* luaState)
{
    math::Vector2d	position = engine::Input::GetCursorPosition<f64>();

    lua_pushnumber(luaState, position.GetX());
    lua_pushnumber(luaState, position.GetY());
    return 2;
}

int script_GeCursorDeltaPos(lua_State* luaState)
{
    math::Vector2d delta = engine::Input::GetCursorDeltaPos<f64>();

    lua_pushnumber(luaState, delta.GetX());
    lua_pushnumber(luaState, delta.GetY());
    return 2;
}

//int script_GetScrollDelta(lua_State* luaState)
//{
//	math::Vector2d scroll = engine::Input::GetScrollDelta<f64>();
//
//	lua_pushnumber(luaState, scroll.GetX());
//	lua_pushnumber(luaState, scroll.GetY());
//	return 2;
//}

void engine::RegisterInputFunctions(lua_State* luaState)
{
    constexpr luaL_Reg inputFuncs[]
    {
        {"IsInputPressed", script_IsInputPressed},
        {"IsInputDown", script_IsInputDown},
        {"IsInputHeld", script_IsInputHeld},
        {"IsInputReleased", script_IsInputReleased},
        {"GetCursorPosition", script_GetCursorPosition},
        {"GetCursorDeltaPos", script_GeCursorDeltaPos},
        //{"GetScrollDelta", script_GetScrollDelta},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, inputFuncs, 0);
    lua_setglobal(luaState, "Input");

}
