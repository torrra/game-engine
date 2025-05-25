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

void engine::RegisterUIButtonFunctions(lua_State* luaState)
{
    constexpr luaL_Reg uiFuncs[]
    {
        {"IsButtonPressed", script_IsButtonPressed},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, uiFuncs, 0);
    lua_setglobal(luaState, "ButtonRef");
}
