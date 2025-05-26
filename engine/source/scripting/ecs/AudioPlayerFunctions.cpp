extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "core/SceneGraph.h"
#include "sounds/AudioPlayer.h"
#include "sounds/Sound.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/ecs/AudioPlayerFunctions.h"

int script_GetAudioPlayerRef(lua_State* luaState)
{
    engine::AudioPlayer* audioPlayer = nullptr;

    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 arugument (handle)");

    else
    {
        engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
        audioPlayer = 
            engine::ScriptSystem::GetCurrentScene()->GetComponent<engine::AudioPlayer>(handle);
    }

    lua_pushlightuserdata(luaState, audioPlayer);
    return 1;
}

int script_GetSound(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*) lua_touserdata(luaState, 1))
    {
        const engine::Sound* sound = audioPlayer->GetSound();

        if (sound)
            lua_pushstring(luaState, sound->GetID().c_str());
        else
            lua_pushnil(luaState);
    }
    else
        lua_pushnil(luaState);

    return 1;
}

int script_GetSoundPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        math::Vector3f position = audioPlayer->GetSoundPosition();

        lua_pushnumber(luaState, static_cast<lua_Number>(position.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(position.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(position.GetZ()));
    }

    return 3;
}

int script_GetSoundVelocity(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        math::Vector3f velocity = audioPlayer->GetSoundVelocity();

        lua_pushnumber(luaState, static_cast<lua_Number>(velocity.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(velocity.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(velocity.GetZ()));
    }

    return 3;
}

int script_GetSoundVolume(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        f32 volume = audioPlayer->GetSoundVolume();

        lua_pushnumber(luaState, static_cast<lua_Number>(volume));
    }

    return 1;
}

int script_GetIs3DSound(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        lua_pushboolean(luaState, audioPlayer->GetIs3DSound());
        return 1;
    }

    return 0;
}

int script_GetListener(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        engine::Listener* listener = audioPlayer->GetListener();
        if (!listener)
            return 0;

        lua_newtable(luaState); // main table

        // position
        lua_pushstring(luaState, "position");
        lua_newtable(luaState);
        lua_pushnumber(luaState, listener->m_position.GetX());
        lua_rawseti(luaState, -2, 1);
        lua_pushnumber(luaState, listener->m_position.GetY());
        lua_rawseti(luaState, -2, 2);
        lua_pushnumber(luaState, listener->m_position.GetZ());
        lua_rawseti(luaState, -2, 3);
        lua_settable(luaState, -3);

        // forward
        lua_pushstring(luaState, "forward");
        lua_newtable(luaState);
        lua_pushnumber(luaState, listener->m_forward.GetX());
        lua_rawseti(luaState, -2, 1);
        lua_pushnumber(luaState, listener->m_forward.GetY());
        lua_rawseti(luaState, -2, 2);
        lua_pushnumber(luaState, listener->m_forward.GetZ());
        lua_rawseti(luaState, -2, 3);
        lua_settable(luaState, -3);

        // up
        lua_pushstring(luaState, "up");
        lua_newtable(luaState);
        lua_pushnumber(luaState, listener->m_up.GetX());
        lua_rawseti(luaState, -2, 1);
        lua_pushnumber(luaState, listener->m_up.GetY());
        lua_rawseti(luaState, -2, 2);
        lua_pushnumber(luaState, listener->m_up.GetZ());
        lua_rawseti(luaState, -2, 3);
        lua_settable(luaState, -3);

        // velocity
        lua_pushstring(luaState, "velocity");
        lua_newtable(luaState);
        lua_pushnumber(luaState, listener->m_velocity.GetX());
        lua_rawseti(luaState, -2, 1);
        lua_pushnumber(luaState, listener->m_velocity.GetY());
        lua_rawseti(luaState, -2, 2);
        lua_pushnumber(luaState, listener->m_velocity.GetZ());
        lua_rawseti(luaState, -2, 3);
        lua_settable(luaState, -3);

        return 1;
    }

    return 0;
}

int script_GetListenerPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        math::Vector3f listenerPosition = audioPlayer->GetListenerPosition();

        lua_pushnumber(luaState, static_cast<lua_Number>(listenerPosition.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(listenerPosition.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(listenerPosition.GetZ()));
    }

    return 3;
}

int script_GetListenerForward(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        math::Vector3f listenerForward = audioPlayer->GetListenerForward();

        lua_pushnumber(luaState, static_cast<lua_Number>(listenerForward.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(listenerForward.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(listenerForward.GetZ()));
    }

    return 3;
}

int script_GetListenerUp(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        math::Vector3f listenerUp = audioPlayer->GetListenerUp();

        lua_pushnumber(luaState, static_cast<lua_Number>(listenerUp.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(listenerUp.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(listenerUp.GetZ()));
    }

    return 3;
}

int script_GetListenerVelocity(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        math::Vector3f listenerVelocity = audioPlayer->GetListenerVelocity();

        lua_pushnumber(luaState, static_cast<lua_Number>(listenerVelocity.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(listenerVelocity.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(listenerVelocity.GetZ()));
    }

    return 3;
}

int script_SetSound(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (self, soundName)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        const char* soundName = luaL_checkstring(luaState, 2);
        audioPlayer->SetSound(soundName);
    }

    return 0;
}

int script_SetSoundPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        f32 xPos = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yPos = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zPos = static_cast<f32>(lua_tonumber(luaState, 4));

        audioPlayer->SetSoundPosition({ xPos, yPos, zPos });
    }

    return 0;
}

int script_SetSoundVelocity(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        f32 xVel = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yVel = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zVel = static_cast<f32>(lua_tonumber(luaState, 4));

        audioPlayer->SetSoundVelocity({ xVel, yVel, zVel });
    }

    return 0;
}

int script_SetSoundVolume(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (self, volume)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        f32 volume = static_cast<f32>(lua_tonumber(luaState, 2));

        audioPlayer->SetVolumeSound(volume);
    }

    return 0;
}

int script_SetIs3DSound(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (self, bool)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        bool is3D = static_cast<bool>(lua_toboolean(luaState, 2));
        audioPlayer->SetIs3DSound(is3D);
    }

    return 0;
}

int script_SetListener(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, listenerData)");

    engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1);
    if (!audioPlayer)
        return 0;

    if (!lua_istable(luaState, 2))
        luaL_error(luaState, "Second argument must be a table");

    math::Vector3f position, forward, up, velocity = math::Vector3f::Zero();

    auto getVec3 = [&](const char* key, math::Vector3f& outVec) {
        lua_getfield(luaState, 2, key);
        if (lua_istable(luaState, -1)) {
            lua_rawgeti(luaState, -1, 1);
            f32 x = (f32)lua_tonumber(luaState, -1); lua_pop(luaState, 1);
            lua_rawgeti(luaState, -1, 2);
            f32 y = (f32)lua_tonumber(luaState, -1); lua_pop(luaState, 1);
            lua_rawgeti(luaState, -1, 3);
            f32 z = (f32)lua_tonumber(luaState, -1); lua_pop(luaState, 1);
            outVec = { x, y, z };
        }
        lua_pop(luaState, 1);
        };

    getVec3("position", position);
    getVec3("forward", forward);
    getVec3("up", up);

    // Check if "velocity" exists and is a table before getting it
    lua_getfield(luaState, 2, "velocity");
    bool hasVelocity = lua_istable(luaState, -1);
    lua_pop(luaState, 1);

    if (hasVelocity)
    {
        getVec3("velocity", velocity);
        audioPlayer->SetListener(position, forward, up, velocity);
    }
    else
    {
        // velocity optionnel, on utilise la surcharge ou la valeur par défaut
        audioPlayer->SetListener(position, forward, up);
    }

    return 0;
}

int script_SetListenerPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        f32 xPos = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yPos = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zPos = static_cast<f32>(lua_tonumber(luaState, 4));

        audioPlayer->SetListenerPosition({ xPos, yPos, zPos });
    }

    return 0;
}

int script_SetListenerForward(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        f32 xPos = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yPos = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zPos = static_cast<f32>(lua_tonumber(luaState, 4));

        audioPlayer->SetListenerForward({ xPos, yPos, zPos });
    }

    return 0;
}

int script_SetListenerUp(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        f32 xPos = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yPos = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zPos = static_cast<f32>(lua_tonumber(luaState, 4));

        audioPlayer->SetListenerUp({ xPos, yPos, zPos });
    }

    return 0;
}

int script_SetListenerVelocity(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        f32 xPos = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yPos = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zPos = static_cast<f32>(lua_tonumber(luaState, 4));

        audioPlayer->SetListenerVelocity({ xPos, yPos, zPos });
    }

    return 0;
}

int script_PlaySound(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);
    if (argumentCount < 1 || argumentCount > 2)
        luaL_error(luaState, "Expected 1 or 2 arguments (self [, is3DSound])");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        bool is3DSound = false;

        if (argumentCount == 2)
            is3DSound = static_cast<bool>(lua_toboolean(luaState, 2));

        audioPlayer->PlaySound(is3DSound);
    }
    return 0;
}

int script_StopSound(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
        audioPlayer->StopSound();
    return 0;
}

int script_PauseSound(lua_State* luaState)
{
    if (lua_gettop(luaState))
        luaL_error(luaState, "Expected 2 arguments (self, isPaused");

    else if (engine::AudioPlayer* audioPlayer = (engine::AudioPlayer*)lua_touserdata(luaState, 1))
    {
        bool isPaused = static_cast<bool>(lua_toboolean(luaState, 2));
        audioPlayer->PauseSound(isPaused);
    }
    return 0;
}

void engine::RegisterAudioPlayerFunctions(lua_State* luaState)
{
    constexpr luaL_Reg audioPlayerFuncs[] =
    {
        {"GetAudioPlayerRef", script_GetAudioPlayerRef},

        {"GetSound", script_GetSound},
        {"GetSoundPosition", script_GetSoundPosition},
        {"GetSoundVelocity", script_GetSoundVelocity},
        {"GetSoundVolume", script_GetSoundVolume},
        {"GetIs3DSound", script_GetIs3DSound},
        {"GetListener", script_GetListener},
        {"GetListenerPosition", script_GetListenerPosition},
        {"GetListenerForward", script_GetListenerForward},
        {"GetListenerUp", script_GetListenerUp},
        {"GetListenerVelocity", script_GetListenerVelocity},

        {"SetSound", script_SetSound},
        {"SetSoundPosition", script_SetSoundPosition},
        {"SetSoundVelocity", script_SetSoundVelocity},
        {"SetSoundVolume", script_SetSoundVolume},
        {"SetIs3DSound", script_SetIs3DSound},
        {"SetListener", script_SetListener},
        {"SetListenerPosition", script_SetListenerPosition},
        {"SetListenerForward", script_SetListenerForward},
        {"SetListenerUp", script_SetListenerUp},
        {"SetListenerVelocity", script_SetListenerVelocity},

        {"PlaySound", script_PlaySound},
        {"StopSound", script_StopSound},
        {"PauseSound", script_PauseSound},

        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, audioPlayerFuncs, 0);
    lua_setglobal(luaState, "AudioPlayerRef");
}
