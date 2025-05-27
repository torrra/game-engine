#pragma once

extern "C"
{
    int script_GetAudioPlayerRef(struct lua_State* luaState);

    int script_GetSound(struct lua_State* luaState);
    int script_GetSoundPosition(struct lua_State* luaState);
    int script_GetSoundVelocity(struct lua_State* luaState);
    int script_GetSoundVolume(struct lua_State* luaState);
    int script_GetIs3DSound(struct lua_State* luaState);
    int script_GetListener(struct lua_State* luaState);
    int script_GetListenerPosition(struct lua_State* luaState);
    int script_GetListenerForward(struct lua_State* luaState);
    int script_GetListenerUp(struct lua_State* luaState);
    int script_GetListenerVelocity(struct lua_State* luaState);

    int script_SetSound(struct lua_State* luaState);
    int script_SetSoundPosition(struct lua_State* luaState);
    int script_SetSoundVelocity(struct lua_State* luaState);
    int script_SetSoundVolume(struct lua_State* luaState);
    int script_SetIs3DSound(struct lua_State* luaState);
    int script_SetListener(struct lua_State* luaState);
    int script_SetListenerPosition(struct lua_State* luaState);
    int script_SetListenerForward(struct lua_State* luaState);
    int script_SetListenerUp(struct lua_State* luaState);
    int script_SetListenerVelocity(struct lua_State* luaState);

    int script_PlaySound(struct lua_State* luaState);
    int script_StopSound(struct lua_State* luaState);
    int script_PauseSound(struct lua_State* luaState);
}

namespace engine
{
    void RegisterAudioPlayerFunctions(struct lua_State* luaState);
}