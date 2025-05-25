#pragma once

extern "C"
{
    int script_SetValue(struct lua_State* luaState);
}

namespace engine
{
    void RegisterUIProgressBarFunctions(struct lua_State* luaState);
}