#pragma once

extern "C"
{
    int script_AngleFromVector3(struct lua_State* luaState);
    int script_CrossVector3(struct lua_State* luaState);

    int script_DistanceFromVector3(struct lua_State* luaState);
    int script_DistanceSquaredFromVector3(struct lua_State* luaState);

    int script_Distance2DFromVector3(struct lua_State* luaState);
    int script_Distance2DSquaredFromVector3(struct lua_State* luaState);

    int script_DotVector3(struct lua_State* luaState);

    int script_IsLongerThanVector3(struct lua_State* luaState);
    int script_IsShorterThanVector3(struct lua_State* luaState);

    int script_IsUnitVector3(struct lua_State* luaState);

    int script_MagnitudeVector3(struct lua_State* luaState);
    int script_MagnitudeSquaredVector3(struct lua_State* luaState);

    int script_NormalizeVector3(struct lua_State* luaState);

    int script_ProjectOntoVector3(struct lua_State* luaState);
    int script_ReflectOntoVector3(struct lua_State* luaState);

    int script_RotateVector3(struct lua_State* luaState);
    int script_ScaleVector3(struct lua_State* luaState);
    int script_TranslateVector3(struct lua_State* luaState);

    int script_LerpVector3(struct lua_State* luaState);
}

namespace engine
{
    void RegisterVector3Functions(struct lua_State* luaState);
}