#include "engine/sounds/AudioPlayer.h"

#pragma region Sounds

#include "engine/sounds/SoundEngine.h"

#pragma endregion

#pragma region Internal

#include "sounds/InternalSoundsStruct.hpp"
#include "sounds/InternalFmodConversion.hpp"
#include "engine/ConsoleLog.hpp"

#pragma endregion

#pragma region Resources

#include "engine/resource/ResourceManager.h"

#pragma endregion

#pragma region FMOD

#include <fmod/fmod_errors.h>

#pragma endregion

#pragma region Serialization

#include "serialization/TextSerializer.h"

#pragma endregion

engine::AudioPlayer::AudioPlayer(EntityHandle inOwner, SceneGraph* inScene)
{
    m_owner         = inOwner;
    m_currentScene  = inScene;
}

math::Vector3f engine::AudioPlayer::GetSoundPosition(void)
{
    auto& channels = SoundEngine::Get().GetSoundImpl().m_channels;
    auto it = channels.find(m_owner);
    
    if (it != channels.end() && it->second)
    {
        FMOD_VECTOR position;
        FMOD_VECTOR velocity;

        FMOD_RESULT result = it->second->get3DAttributes(&position, &velocity);
        if (result == FMOD_OK)
        {
            m_position = ToMathVector(position);
            return ToMathVector(position);
        }
        else
            PrintLog(ErrorPreset(), std::string(FMOD_ErrorString(result)));
    }

    return math::Vector3f::Zero();
}

math::Vector3f engine::AudioPlayer::GetSoundVelocity(void)
{
    auto& channels = SoundEngine::Get().GetSoundImpl().m_channels;
    auto it = channels.find(m_owner);

    if (it != channels.end() && it->second)
    {
        FMOD_VECTOR position;
        FMOD_VECTOR velocity;

        FMOD_RESULT result = it->second->get3DAttributes(&position, &velocity);
        if (result == FMOD_OK)
        {
            m_velocity = ToMathVector(velocity);

            return ToMathVector(velocity);
        }
        else
            PrintLog(ErrorPreset(), std::string(FMOD_ErrorString(result)));
    }

    return math::Vector3f::Zero();
}

f32 engine::AudioPlayer::GetSoundVolume(void)
{
    auto& channels = SoundEngine::Get().GetSoundImpl().m_channels;
    auto it = channels.find(m_owner);

    if (it != channels.end() && it->second)
    {
        f32 volume;

        FMOD_RESULT result = it->second->getVolume(&volume);

        if (result == FMOD_OK)
        {
            m_volume = volume;
            return volume;
        }
        else
            PrintLog(ErrorPreset(), std::string(FMOD_ErrorString(result)));
    }
    return -1.f;
}

const engine::Sound* engine::AudioPlayer::GetSound(void) const
{
    return m_sound;
}

bool engine::AudioPlayer::GetIs3DSound(void) const
{
    return m_is3DSound;
}

bool engine::AudioPlayer::GetIsPlayed(void) const
{
    return m_isPlayed;
}

math::Vector3f engine::AudioPlayer::GetListenerPosition(void) const
{
    if (m_listener != nullptr)
    {
        return m_listener->m_position;
}
    return {};
}

math::Vector3f engine::AudioPlayer::GetListenerForward(void) const
{
    if (m_listener != nullptr)
    {
        return m_listener->m_forward;
}
    return {};
}

math::Vector3f engine::AudioPlayer::GetListenerUp(void) const
{
    if (m_listener != nullptr)
    {
        return m_listener->m_up;
}
    return {};
}

math::Vector3f engine::AudioPlayer::GetListenerVelocity(void) const
{
    if (m_listener != nullptr)
    {
        return m_listener->m_velocity;
    }
    return {};
}

void engine::AudioPlayer::SetVolumeSound(f32 inVolume)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(m_owner);
    
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

        m_volume = inVolume;
    }
}

void engine::AudioPlayer::SetSoundPosition(const math::Vector3f& inPosition)
{
    auto& channels = SoundEngine::Get().GetSoundImpl().m_channels;
    auto it = channels.find(m_owner);

    if (it != channels.end() && it->second)
    {
        FMOD_VECTOR position;
        FMOD_VECTOR velocity;

        FMOD_RESULT res = it->second->get3DAttributes(&position, &velocity);

        if (res == FMOD_OK)
        {
            position = ToFmodVector(inPosition);

            FMOD_RESULT result = it->second->set3DAttributes(&position, &velocity);
            if (result == FMOD_OK)
            {
                PrintLog(SuccessPreset(), "Position successfully setted");
                m_position = inPosition;
            }
            else
                PrintLog(ErrorPreset(), std::string(FMOD_ErrorString(result)));
        }
        else
            PrintLog(ErrorPreset(), std::string(FMOD_ErrorString(res)));
    }
}

void engine::AudioPlayer::SetSoundVelocity(const math::Vector3f& inVelocity)
{
    auto& channels = SoundEngine::Get().GetSoundImpl().m_channels;
    auto it = channels.find(m_owner);

    if (it != channels.end() && it->second)
    {
        FMOD_VECTOR position;
        FMOD_VECTOR velocity;

        FMOD_RESULT res = it->second->get3DAttributes(&position, &velocity);

        if (res == FMOD_OK)
        {
            velocity = ToFmodVector(inVelocity);

            FMOD_RESULT result = it->second->set3DAttributes(&position, &velocity);
            if (result == FMOD_OK)
            {
                PrintLog(SuccessPreset(), "Position successfully setted");
                m_velocity = inVelocity;
            }
            else
                PrintLog(ErrorPreset(), std::string(FMOD_ErrorString(result)));
        }
        else
            PrintLog(ErrorPreset(), std::string(FMOD_ErrorString(res)));
    }
}

void engine::AudioPlayer::SetListener(const math::Vector3f& inPosition, 
                                              const math::Vector3f& inForward, 
                                              const math::Vector3f& inUp, 
                                              const math::Vector3f& inVelocity)
{
    const   FMOD_VECTOR position    = ToFmodVector(inPosition);
    const   FMOD_VECTOR forward     = ToFmodVector(inForward);
    const   FMOD_VECTOR up          = ToFmodVector(inUp);
            FMOD_VECTOR velocity    = ToFmodVector(inVelocity);
    
    if (m_listener != nullptr)
    {
        m_listener->m_position = inPosition;
        m_listener->m_forward = inForward;
        m_listener->m_up = inUp;
        m_listener->m_velocity = inVelocity;
    }

    SoundEngine::Get().GetSoundImpl().m_system->set3DListenerAttributes(0, &position, &velocity,
                                                                        &forward, &up);
}

void engine::AudioPlayer::SetListenerPosition(const math::Vector3f& inPosition)
{
    FMOD_VECTOR position    = FMOD_VECTOR(0.f);
    FMOD_VECTOR forward     = FMOD_VECTOR(0.f);
    FMOD_VECTOR up          = FMOD_VECTOR(0.f);
    FMOD_VECTOR velocity    = FMOD_VECTOR(0.f);

    if (m_listener != nullptr)
    {
        m_listener->m_position = inPosition;
}

    SoundEngine::Get().GetSoundImpl().m_system->get3DListenerAttributes(
        0, &position, &velocity, &forward, &up);

    position = ToFmodVector(inPosition);

    SoundEngine::Get().GetSoundImpl().m_system->set3DListenerAttributes(0, &position, &velocity,
        &forward, &up);
}

void engine::AudioPlayer::SetListenerForward(const math::Vector3f& inForward)
{
    FMOD_VECTOR position    = FMOD_VECTOR(0.f);
    FMOD_VECTOR forward     = FMOD_VECTOR(0.f);
    FMOD_VECTOR up          = FMOD_VECTOR(0.f);
    FMOD_VECTOR velocity    = FMOD_VECTOR(0.f);

    if (m_listener != nullptr)
    {
        m_listener->m_forward = inForward;
}

    SoundEngine::Get().GetSoundImpl().m_system->get3DListenerAttributes(
        0, &position, &velocity, &forward, &up);

    forward = ToFmodVector(inForward);

    SoundEngine::Get().GetSoundImpl().m_system->set3DListenerAttributes(0, &position, &velocity,
        &forward, &up);
}

void engine::AudioPlayer::SetListenerUp(const math::Vector3f& inUp)
{
    FMOD_VECTOR position    = FMOD_VECTOR(0.f);
    FMOD_VECTOR forward     = FMOD_VECTOR(0.f);
    FMOD_VECTOR up          = FMOD_VECTOR(0.f);
    FMOD_VECTOR velocity    = FMOD_VECTOR(0.f);

    if (m_listener != nullptr)
    {
        m_listener->m_up = inUp;
}

    SoundEngine::Get().GetSoundImpl().m_system->get3DListenerAttributes(
        0, &position, &velocity, &forward, &up);

    up = ToFmodVector(inUp);

    SoundEngine::Get().GetSoundImpl().m_system->set3DListenerAttributes(0, &position, &velocity,
        &forward, &up);
}

void engine::AudioPlayer::SetListenerVelocity(const math::Vector3f& inVelocity)
{
    FMOD_VECTOR position    = FMOD_VECTOR(0.f);
    FMOD_VECTOR forward     = FMOD_VECTOR(0.f);
    FMOD_VECTOR up          = FMOD_VECTOR(0.f);
    FMOD_VECTOR velocity    = FMOD_VECTOR(0.f);

    if (m_listener != nullptr)
    {
        m_listener->m_velocity = inVelocity;
    }

    SoundEngine::Get().GetSoundImpl().m_system->get3DListenerAttributes(
        0, &position, &velocity, &forward, &up);

    velocity = ToFmodVector(inVelocity);

    SoundEngine::Get().GetSoundImpl().m_system->set3DListenerAttributes(0, &position, &velocity,
        &forward, &up);
}

void engine::AudioPlayer::SetIs3DSound(bool inIs3DSound)
{
    m_is3DSound = inIs3DSound;

    if (inIs3DSound)
        m_listener = new Listener();
}

void engine::AudioPlayer::SetIsPlayed(bool inIsPlayed)
{
    m_isPlayed = inIsPlayed;
}

void engine::AudioPlayer::SetSound(const char* inSoundName)
{
    if (const Sound* sound = ResourceManager::GetResource<Sound>(inSoundName))
        SetSound(sound);
}

void engine::AudioPlayer::SetSound(const Sound* inSound)
{
    m_sound = inSound;
}

void engine::AudioPlayer::PlaySoundWithout3D(void)
{
    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = SoundEngine::Get().GetSoundImpl().m_system->playSound(static_cast<FMOD::Sound*>(m_sound->GetSound()), nullptr,
                                                                               true, &channel);
    
    if (result != FMOD_OK)
        return;
    
    if (channel)
    {
        FMOD_RESULT result1 = channel->setVolume(m_volume);
    
        if (result1 != FMOD_OK)
            return;
    
        FMOD_RESULT result2 = channel->setPaused(false);
    
        if (result2 != FMOD_OK)
            return;
    
        SoundEngine::Get().GetSoundImpl().m_channels[m_owner] = channel;
    }
    m_is3DSound = false;
}

void engine::AudioPlayer::PlaySound3D(void)
{    
    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = SoundEngine::Get().GetSoundImpl().m_system->playSound(static_cast<FMOD::Sound*>(m_sound->GetSound()),
                                                                                nullptr, true, 
                                                                                &channel);
    
    if (result != FMOD_OK)
        return;
    
    channel->setMode(FMOD_3D);
    const FMOD_VECTOR velocity = ToFmodVector(m_velocity);
    const FMOD_VECTOR position = ToFmodVector(m_position);
    channel->set3DAttributes(&position, &velocity);
    channel->setVolume(m_volume);
    channel->setPaused(false);
    
    SoundEngine::Get().GetSoundImpl().m_channels[m_owner] = channel;

    m_is3DSound = true;
}

void engine::AudioPlayer::PlaySound(bool is3DSound)
{
    if (is3DSound)
    {
        if (m_currentScene->GetEntity(m_owner)->HasComponent<Transform>())
        {
            m_position = m_currentScene->GetComponent<Transform>(m_owner)->GetPosition();
            PlaySound3D();
        }
        else
        {
            PlaySound3D();
        }
    }
    else
    {
        PlaySoundWithout3D();
    }
}

void engine::AudioPlayer::StopSound(void)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(m_owner);

    if (it != SoundEngine::Get().GetSoundImpl().m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->stop();

        if (result != FMOD_OK)
            return;
    }
}

void engine::AudioPlayer::PauseSound(bool inIsPaused)
{
    auto it = SoundEngine::Get().GetSoundImpl().m_channels.find(m_owner);
    
    if (it != SoundEngine::Get().GetSoundImpl().m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->setPaused(inIsPaused);
    
        if (result != FMOD_OK)
            return;
    }
}

void engine::AudioPlayer::Invalidate(void)
{
    SoundEngine::Get().GetSoundImpl().m_channels.erase(m_owner);
}

}
}