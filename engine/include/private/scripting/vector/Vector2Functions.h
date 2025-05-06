#pragma once

extern "C"
{
    int script_AngleFromVector2(struct lua_State* luaState);
    int script_CrossVector2(struct lua_State* luaState);

    int script_DistanceFromVector2(struct lua_State* luaState);
    int script_DistanceSquaredFromVector2(struct lua_State* luaState);

    int script_DotVector2(struct lua_State* luaState);

    int script_IsLongerThanVector2(struct lua_State* luaState);
    int script_IsShorterThanVector2(struct lua_State* luaState);

    int script_IsUnitVector2(struct lua_State* luaState);

    int script_MagnitudeVector2(struct lua_State* luaState);
    int script_MagnitudeSquaredVector2(struct lua_State* luaState);

    int script_NormalizeVector2(struct lua_State* luaState);
    int script_NormalVector2(struct lua_State* luaState);

    int script_ProjectOntoVector2(struct lua_State* luaState);
    int script_ReflectOntoVector2(struct lua_State* luaState);

    int script_RotateVector2(struct lua_State* luaState);
    int script_ScaleVector2(struct lua_State* luaState);
}

namespace engine
{
    void RegisterVector2Functions(struct lua_State* luaState);
}