extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "scripting/ui/ProgressBar.h"
#include "ui/elements/ProgressBar.h"
#include <math/Arithmetic.hpp>

int script_SetValue(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (element, value)");
    else if(engine::ProgressBar* progressBar = (engine::ProgressBar*) lua_touserdata(luaState, 1))
    {
        f32 value = static_cast<f32>(lua_tonumber(luaState, 2));
        progressBar->SetValue(value);
    }

    return 0;
}

int script_SetBgColor(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 arguments (element, red, green, blue, alpha)");
    else if (engine::ProgressBar* progressBar = (engine::ProgressBar*) lua_touserdata(luaState, 1))
    {
        progressBar->SetBgColor(
            static_cast<f32>(lua_tonumber(luaState, 2)), // red
            static_cast<f32>(lua_tonumber(luaState, 3)), // green
            static_cast<f32>(lua_tonumber(luaState, 4)), // blue
            static_cast<f32>(lua_tonumber(luaState, 5))  // alpha
        );
    }

    return 0;
}

int script_SetFillColor(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 arguments (element, red, green, blue, alpha)");
    else if (engine::ProgressBar* progressBar = (engine::ProgressBar*) lua_touserdata(luaState, 1))
    {
        progressBar->SetFillColor(
            static_cast<f32>(lua_tonumber(luaState, 2)), // red
            static_cast<f32>(lua_tonumber(luaState, 3)), // green
            static_cast<f32>(lua_tonumber(luaState, 4)), // blue
            static_cast<f32>(lua_tonumber(luaState, 5))  // alpha
        );
    }

    return 0;
}

void engine::RegisterUIProgressBarFunctions(lua_State* luaState)
{
    constexpr luaL_Reg uiFuncs[]
    {
        {"SetValue", script_SetValue},
        {"SetBgColor", script_SetBgColor},
        {"SetFillColor", script_SetFillColor},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, uiFuncs, 0);
    lua_setglobal(luaState, "ProgressBarRef");
}