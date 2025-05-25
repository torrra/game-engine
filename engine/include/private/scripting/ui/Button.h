#pragma once

extern "C"
{
    int script_IsButtonPressed(struct lua_State* luaState);
}

namespace engine
{
    void RegisterUIButtonFunctions(struct lua_State* luaState);
}