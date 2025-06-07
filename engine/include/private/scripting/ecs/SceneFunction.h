#pragma once

extern "C"
{
    int script_CreateEntity(struct lua_State* luaState);
    int script_DestroyEntity(struct lua_State* luaState);
    int script_CreateComponent(struct lua_State* luaState);
    int script_DestroyComponent(struct lua_State* luaState);
}

namespace engine
{
    void RegisterSceneFunctions(struct lua_State* luaState);
}