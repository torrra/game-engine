#include "engine/sounds/Sound.h"

#pragma region Sounds

#include "engine/sounds/SoundsEngine.h"

#pragma endregion

#pragma region Internal

#include "sounds/InternalSoundsStruct.hpp"

#pragma endregion

#pragma region Resources

#include "engine/resource/ResourceManager.h"

#pragma endregion

bool engine::Sound::GetIsLooping(void) const
{
    return m_isLooping;
}

std::string engine::Sound::GetID(void) const
{
    return m_ID;
}

void* engine::Sound::GetSound(void) const
{
    return m_sound;
}

void engine::Sound::SetLooping(bool inIsLooping)
{
    m_isLooping = inIsLooping;
}

bool engine::Sound::LoadResource(const char* inPath)
{
    m_ID = inPath;

    FMOD_MODE mode = FMOD_DEFAULT;
    if (m_isLooping)
        mode |= FMOD_LOOP_NORMAL;

    FMOD_RESULT result = SoundEngine::Get().GetSoundImpl().m_system->createSound(m_ID.c_str(),
        mode, nullptr, reinterpret_cast<FMOD::Sound**>(&m_sound));

    if (result != FMOD_OK)
        return false;

    return true;
}
