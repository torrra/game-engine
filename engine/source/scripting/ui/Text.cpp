extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "scripting/ui/Text.h"
#include "ui/elements/Label.h"

int script_SetTextColor(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 argument (element, red, green, blue, alpha)");
    else if (engine::Label* label = (engine::Label*)lua_touserdata(luaState, 1))
    {
        printf("Call success\n");
        f32 red = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 green = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 blue = static_cast<f32>(lua_tonumber(luaState, 4));
        f32 alpha = static_cast<f32>(lua_tonumber(luaState, 5));

        label->SetTextColor(red, green, blue, alpha);
    }

    return 0;
}

void engine::RegisterUITextFunctions(lua_State* luaState)
{
    constexpr luaL_Reg uiFuncs[]
    {
        {"SetTextColor", script_SetTextColor},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, uiFuncs, 0);
    lua_setglobal(luaState, "TextRef");
}
