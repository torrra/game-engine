extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "ui/elements/Button.h"
#include "scripting/ui/Button.h"

int script_IsButtonPressed(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (element)");
    else if (engine::Button* button = (engine::Button*) lua_touserdata(luaState, 1))
    {
        lua_pushboolean(luaState, button->IsPressed());
    }

    return 1;
}

int script_SetColor(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 argument (element, red, green, blue, alpha)");
    else if (engine::Button* button = (engine::Button*) lua_touserdata(luaState, 1))
    {
        button->SetColor(
            static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3)),
            static_cast<f32>(lua_tonumber(luaState, 4)),
            static_cast<f32>(lua_tonumber(luaState, 5))
        );
    }

    return 0;
}

int script_SetHoveredColor(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 argument (element, red, green, blue, alpha)");
    else if (engine::Button* button = (engine::Button*) lua_touserdata(luaState, 1))
    {
        button->SetHoveredColor(
            static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3)),
            static_cast<f32>(lua_tonumber(luaState, 4)),
            static_cast<f32>(lua_tonumber(luaState, 5))
        );
    }

    return 0;
}

int script_SetPressedColor(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 argument (element, red, green, blue, alpha)");
    else if (engine::Button* button = (engine::Button*) lua_touserdata(luaState, 1))
    {
        button->SetPressedColor(
            static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3)),
            static_cast<f32>(lua_tonumber(luaState, 4)),
            static_cast<f32>(lua_tonumber(luaState, 5))
        );
    }

    return 0;
}

int script_SetButtonSize(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 3)
        luaL_error(luaState, "Expected 3 argument (element, width, height)");
    else if (engine::Button* button = (engine::Button*) lua_touserdata(luaState, 1))
    {
        button->SetScale({
            static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3))
        });
    }

    return 0;
}

void engine::RegisterUIButtonFunctions(lua_State* luaState)
{
    constexpr luaL_Reg uiFuncs[]
    {
        {"IsButtonPressed", script_IsButtonPressed},
        {"SetColor", script_SetColor},
        {"SetHoveredColor", script_SetHoveredColor},
        {"SetPressedColor", script_SetPressedColor},
        {"SetButtonSize", script_SetButtonSize},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, uiFuncs, 0);
    lua_setglobal(luaState, "ButtonRef");
}
