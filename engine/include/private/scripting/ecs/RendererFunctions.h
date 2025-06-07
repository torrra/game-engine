#pragma once

extern "C"
{
    int script_GetRendererRef(struct lua_State* luaState);

    int script_SetRendererMaterial(struct lua_State* luaState);
    int script_SetRendererModel(struct lua_State* luaState);
    int script_SetRendererShader(struct lua_State* luaState);

    int script_GetRendererMaterial(struct lua_State* luaState);
    int script_GetRendererModel(struct lua_State* luaState);
    int script_GetRendererShader(struct lua_State* luaState);

    int script_SetAnimatorAnimation(struct lua_State* luaState);
    int script_PauseAnimatorAnimation(struct lua_State* luaState);
    int script_StopAnimatorAnimation(struct lua_State* luaState);
    int script_PlayAnimatorAnimation(struct lua_State* luaState);

    int script_GetAnimatorAnimation(struct lua_State* luaState);

    int script_SetAnimatorKeyFrame(struct lua_State* luaState);
    int script_SetAnimatorAnimationSpeed(struct lua_State* luaState);

    int script_GetAnimatorKeyFrame(struct lua_State* luaState);
    int script_GetAnimatorAnimationSpeed(struct lua_State* luaState);

    int script_IsAnimatorLooped(struct lua_State* luaState);
    int script_SetAnimatorLooped(struct lua_State* luaState);

    int script_IsAnimatorPlaying(struct lua_State* luaState);
    int script_IsAnimatorPaused(struct lua_State* luaState);
}

namespace engine
{
    void RegisterRendererFunctions(struct lua_State* luaState);
}