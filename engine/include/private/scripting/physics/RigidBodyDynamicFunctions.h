#pragma once

extern "C"
{
    int script_GetRigidBodyDynamicRef(struct lua_State* luaState);

    int script_GetIsGravityDisabled(struct lua_State* luaState);
    int script_GetBoxHalfExtents(struct lua_State* luaState);
    int script_GetSphereRadius(struct lua_State* luaState);
    int script_GetCapsuleFormat(struct lua_State* luaState);
    int script_GetLinearVelocity(struct lua_State* luaState);
    int script_GetAngularVelocity(struct lua_State* luaState);
    int script_GetIsTrigger(struct lua_State* luaState);
    int script_GetIsXAxisLock(struct lua_State* luaState);
    int script_GetIsYAxisLock(struct lua_State* luaState);
    int script_GetIsZAxisLock(struct lua_State* luaState);

    int script_SetGravityDisabled(struct lua_State* luaState);
    int script_SetBoxHalfExtents(struct lua_State* luaState);
    int script_SetSphereRadius(struct lua_State* luaState);
    int script_SetCapsuleFormat(struct lua_State* luaState);
    int script_SetTrigger(struct lua_State* luaState);
    int script_SetLinearVelocity(struct lua_State* luaState);
    int script_SetAngularVelocity(struct lua_State* luaState);
    int script_SetXAxisLock(struct lua_State* luaState);
    int script_SetYAxisLock(struct lua_State* luaState);
    int script_SetZAxisLock(struct lua_State* luaState);

    int script_AddForce(struct lua_State* luaState);
    int script_AddTorque(struct lua_State* luaState);
}

namespace engine
{
    void RegisterRigidBodyDynamicFunctions(struct lua_State* luaState);
}