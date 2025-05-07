#include "engine/sounds/AudioPlayer.h"

#pragma region Sounds

#include "engine/sounds/SoundsEngine.h"

#pragma endregion

#pragma region Internal

#include "sounds/InternalSoundsStruct.hpp"
#include "sounds/InternalFmodConversion.hpp"

#pragma endregion

engine::AudioPlayer::AudioPlayer(EntityHandle inOwner, SceneGraph* inScene)
{
    m_owner         = inOwner;
    m_currentScene  = inScene;
}

void engine::AudioPlayer::SetVolumeSound(const std::string& inID, f32 inVolume)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(inID);
    
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

bool engine::AudioPlayer::LoadSound(const std::string& inID, const std::string& inPath, bool inLoop)
{
    if (SoundEngine::Get().GetSoundImpl().m_sounds.find(inID) != 
        SoundEngine::Get().GetSoundImpl().m_sounds.end())
            return true;
        
    FMOD_MODE mode = FMOD_DEFAULT;
    if (inLoop)
        mode |= FMOD_LOOP_NORMAL;
        
    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = SoundEngine::Get().GetSoundImpl().m_system->createSound(inPath.c_str(),
                                                                                 mode, nullptr, 
                                                                                 &sound);
    if (result != FMOD_OK)
        return false;
        
    SoundEngine::Get().GetSoundImpl().m_sounds[inID] = sound;
        
    return true;
}

void engine::AudioPlayer::PlaySound(const std::string& inID, f32 inVolume, bool inPaused)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_sounds.find(inID);
    
    if (it == SoundEngine::Get().GetSoundImpl().m_sounds.end())
        return;
    
    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = SoundEngine::Get().GetSoundImpl().m_system->playSound(it->second, nullptr,
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
    
        SoundEngine::Get().GetSoundImpl().m_channels[inID] = channel;
    }
}

void engine::AudioPlayer::PlaySound3D(const std::string& inID, const math::Vector3f& inPosition, 
                                      const math::Vector3f& inVelocity, f32 inVolume)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_sounds.find(inID);
    
        if (it == SoundEngine::Get().GetSoundImpl().m_sounds.end())
            return;
    
        FMOD::Channel* channel = nullptr;
        FMOD_RESULT result = SoundEngine::Get().GetSoundImpl().m_system->playSound(it->second, 
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
    
        SoundEngine::Get().GetSoundImpl().m_channels[inID] = channel;
}

void engine::AudioPlayer::StopSound(const std::string& inID)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(inID);

    if (it != SoundEngine::Get().GetSoundImpl().m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->stop();

        if (result != FMOD_OK)
            return;
    }
}

void engine::AudioPlayer::PauseSound(const std::string& inID, bool inIsPaused)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(inID);
    
    if (it != SoundEngine::Get().GetSoundImpl().m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->setPaused(inIsPaused);
    
        if (result != FMOD_OK)
            return;
    }
}
