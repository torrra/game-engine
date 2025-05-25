extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "scripting/ui/Element.h"
#include "ui/UIElement.h"

int script_GetPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (element)");

    return 2;
}

int script_GetScale(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (element)");

    return 2;
}

int script_SetPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 3)
        luaL_error(luaState, "Expected 3 argument (element, xPosition, yPosition)");

    return 0;
}

int script_SetScale(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 3)
        luaL_error(luaState, "Expected 3 argument (element, xScale, yScale)");

    return 0;
}

int script_SetAutoScale(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 argument (element, shouldAutoScale)");

    return 0;
}

void engine::RegisterUIElementFunctions(lua_State* luaState)
{
    constexpr luaL_Reg uiFuncs[]
    {
        {"GetPosition", script_GetPosition},
        {"GetScale", script_GetScale},
        {"SetPosition", script_SetPosition},
        {"SetScale", script_SetScale},
        {"SetAutoScale", script_SetAutoScale},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, uiFuncs, 0);
    lua_setglobal(luaState, "elementRef");
}
