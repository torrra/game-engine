#include "engine/sounds/AudioPlayer.h"

#pragma region Sounds

#include "engine/sounds/SoundsEngine.h"

#pragma endregion

#pragma region Internal

#include "sounds/InternalSoundsStruct.hpp"
#include "sounds/InternalFmodConversion.hpp"

#pragma endregion

#pragma region Resources

#include "engine/resource/ResourceManager.h"

#pragma endregion

engine::AudioPlayer::AudioPlayer(EntityHandle inOwner, SceneGraph* inScene)
{
    m_owner         = inOwner;
    m_currentScene  = inScene;
}

void engine::AudioPlayer::SetVolumeSound(f32 inVolume)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(m_sound->GetID());
    
    if (inVolume < 0.f)
    {
        inVolume = 0.f;
    }
    else if (inVolume > 1.f)
    {
        inVolume = 1.f;
    }
    
    if (it != SoundEngine::Get().GetSoundImpl().m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->setVolume(inVolume);
    
        if (result != FMOD_OK)
            return;
    }
}

void engine::AudioPlayer::SetListenerPosition(const math::Vector3f& inPosition, 
                                              const math::Vector3f& inForward, 
                                              const math::Vector3f& inUp, 
                                              const math::Vector3f& inVelocity)
{
    const   FMOD_VECTOR position    = ToFmodVector(inPosition);
    const   FMOD_VECTOR forward     = ToFmodVector(inForward);
    const   FMOD_VECTOR up          = ToFmodVector(inUp);
            FMOD_VECTOR velocity    = ToFmodVector(inVelocity);
    
    SoundEngine::Get().GetSoundImpl().m_system->set3DListenerAttributes(0, &position, &velocity,
                                                                        &forward, &up);
}

void engine::AudioPlayer::SetSound(const Sound* inSound)
{
    m_sound = inSound;
}

void engine::AudioPlayer::PlaySound(f32 inVolume, bool inPaused)
{
    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = SoundEngine::Get().GetSoundImpl().m_system->playSound(static_cast<FMOD::Sound*>(m_sound->GetSound()), nullptr,
                                                                               true, &channel);
    
    if (result != FMOD_OK)
        return;
    
    if (channel)
    {
        FMOD_RESULT result1 = channel->setVolume(inVolume);
    
        if (result1 != FMOD_OK)
            return;
    
        FMOD_RESULT result2 = channel->setPaused(inPaused);
    
        if (result2 != FMOD_OK)
            return;
    
        SoundEngine::Get().GetSoundImpl().m_channels[m_sound->GetID()] = channel;
    }
}

void engine::AudioPlayer::PlaySound3D(const math::Vector3f& inPosition, 
                                      const math::Vector3f& inVelocity, f32 inVolume)
{    
    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = SoundEngine::Get().GetSoundImpl().m_system->playSound(static_cast<FMOD::Sound*>(m_sound->GetSound()),
                                                                                nullptr, true, 
                                                                                &channel);
    
    if (result != FMOD_OK)
        return;
    
    channel->setMode(FMOD_3D);
    const FMOD_VECTOR velocity = ToFmodVector(inVelocity);
    const FMOD_VECTOR position = ToFmodVector(inPosition);
    channel->set3DAttributes(&position, &velocity);
    channel->setVolume(inVolume);
    channel->setPaused(false);
    
    SoundEngine::Get().GetSoundImpl().m_channels[m_sound->GetID()] = channel;
}

void engine::AudioPlayer::StopSound(void)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(m_sound->GetID());

    if (it != SoundEngine::Get().GetSoundImpl().m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->stop();

        if (result != FMOD_OK)
            return;
    }
}

void engine::AudioPlayer::PauseSound(bool inIsPaused)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(m_sound->GetID());
    
    if (it != SoundEngine::Get().GetSoundImpl().m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->setPaused(inIsPaused);
    
        if (result != FMOD_OK)
            return;
    }
}
