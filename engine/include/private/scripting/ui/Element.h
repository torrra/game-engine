#pragma once

extern "C"
{
    int script_GetPosition(struct lua_State* luaState);
    int script_GetScale(struct lua_State* luaState);

    int script_SetPosition(struct lua_State* luaState);
    int script_SetScale(struct lua_State* luaState);
    int script_SetAutoScale(struct lua_State* luaState);

}

namespace engine
{
    void RegisterUIElementFunctions(struct lua_State* luaState);
}