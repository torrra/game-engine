#pragma once

extern "C"
{
    int script_GetNavPointRef(struct lua_State* luaState);

    int script_GetNavPointPosition(struct lua_State* luaState);

    int script_SetNavPointPosition(struct lua_State* luaState);
}

namespace engine
{
    void RegisterNavPointFunctions(struct lua_State* luaState);

} // !Namespace engine