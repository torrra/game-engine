#pragma once

extern "C"
{
    int script_GetRigidBodyStaticRef(struct lua_State* luaState);

    int script_GetStaticBoxHalfExtents(struct lua_State* luaState);
    int script_GetStaticSphereRadius(struct lua_State* luaState);
    int script_GetStaticCapsuleFormat(struct lua_State* luaState);
    int script_GetStaticIsTrigger(struct lua_State* luaState);

    int script_SetStaticBoxHalfExtents(struct lua_State* luaState);
    int script_SetStaticSphereRadius(struct lua_State* luaState);
    int script_SetStaticCapsuleFormat(struct lua_State* luaState);
    int script_SetStaticTrigger(struct lua_State* luaState);
}

namespace engine
{
    void RegisterRigidBodyStaticFunctions(struct lua_State* luaState);
}