#pragma once

extern "C"
{
    int script_LoadResource(struct lua_State* luaState);
}

namespace engine
{
    void RegisterResourceFunctions(struct lua_State* luaState);
}