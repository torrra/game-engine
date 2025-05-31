#pragma once

extern "C"
{
    int script_SetTextColor(lua_State* luaState);
}

namespace engine
{
    void RegisterUITextFunctions(struct lua_State* luaState);
}