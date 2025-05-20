#pragma once

extern "C"
{
    int script_GetRaycastOrigin(struct lua_State* luaState);
    int script_GetRaycastDirection(struct lua_State* luaState);
    int script_GetRaycastDistance(struct lua_State* luaState);

    int script_SetRaycastOrigin(struct lua_State* luaState);
    int script_SetRaycastDirection(struct lua_State* luaState);
    int script_SetRaycastDistance(struct lua_State* luaState);

    int script_HasRaycastHit(struct lua_State* luaState);

    int script_CreateRaycast(struct lua_State* luaState);
}

namespace engine
{
    void RegisterRaycastFunctions(struct lua_State* luaState);
}