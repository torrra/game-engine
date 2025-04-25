#include "engine/sounds/SoundsEngine.h"

#pragma region Internal

#include "sounds/InternalSoundsStruct.hpp"
#include "sounds/InternalFmodConversion.hpp"

#pragma endregion

#include "engine/ConsoleLog.hpp"

engine::SoundsEngine::SoundsEngine(void)
{
    m_soundsImpl    = new SoundsImpl();
}

engine::SoundsEngine::~SoundsEngine(void)
{
    delete m_soundsImpl;
    m_soundsImpl = nullptr;
}

void engine::SoundsEngine::SetPositionSound3D(const std::string& inID, 
                                              const math::Vector3f& inPosition, 
                                              const math::Vector3f& inForward, 
                                              const math::Vector3f& inUp)
{
    inID;
    const FMOD_VECTOR position  = ToFmodVector(inPosition);
    const FMOD_VECTOR forward   = ToFmodVector(inForward);
    const FMOD_VECTOR up        = ToFmodVector(inUp);
    FMOD_VECTOR velocity        = { 1, 0, 0 };

    m_soundsImpl->m_system->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
}

engine::SoundsImpl* engine::SoundsEngine::GetSoundsImpl(void) const
{
    return m_soundsImpl;
}

bool engine::SoundsEngine::InitSoundsEngine(void)
{
    FMOD_RESULT result = FMOD::System_Create(&m_soundsImpl->m_system);
    if (result != FMOD_OK)
        return false;

    result = m_soundsImpl->m_system->init(512, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK)
        return false;

    return true;
}

void engine::SoundsEngine::UpdateSoundsEngine(void)
{
    if (!m_soundsImpl->m_system)
        return;

    m_soundsImpl->m_system->update();
}

bool engine::SoundsEngine::LoadSound(const std::string& inID, const std::string& inPath, 
                                     bool inLoop)
{
    if (m_soundsImpl->m_sounds.find(inID) != m_soundsImpl->m_sounds.end())
        return true;

    FMOD_MODE mode = FMOD_DEFAULT;
    if (inLoop)
        mode |= FMOD_LOOP_NORMAL;

    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = m_soundsImpl->m_system->createSound(inPath.c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK)
        return false;

    m_soundsImpl->m_sounds[inID] = sound;

    return true;
}

void engine::SoundsEngine::PlaySound(const std::string& inID)
{
    auto it = m_soundsImpl->m_sounds.find(inID);

    if (it == m_soundsImpl->m_sounds.end())
        return;

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = m_soundsImpl->m_system->playSound(it->second, nullptr, true, &channel);

    if (result != FMOD_OK)
        return;

    if (channel)
    {
        FMOD_RESULT result1 = channel->setVolume(1.0f);

        if (result1 != FMOD_OK)
            return;

        FMOD_RESULT result2 = channel->setPaused(false);

        if (result2 != FMOD_OK)
            return;

        m_soundsImpl->m_channels[inID] = channel;
    }
}

void engine::SoundsEngine::PlaySound3D(const std::string& inID, const math::Vector3f& inPosition)
{
    auto it = m_soundsImpl->m_sounds.find(inID);

    if (it == m_soundsImpl->m_sounds.end())
        return;

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = m_soundsImpl->m_system->playSound(it->second, nullptr, true, &channel);

    if (result != FMOD_OK)
    {
        PrintLog(ErrorPreset(), "Failed to play sound");
        return;
    }

    channel->setMode(FMOD_3D);
    channel->setVolume(1.0f);
    channel->setPaused(false);
    const FMOD_VECTOR& velocity = { 100, 0, 0 };
    const FMOD_VECTOR& position = ToFmodVector(inPosition);
    channel->set3DAttributes(&position, &velocity);
    m_soundsImpl->m_channels[inID] = channel;
    PrintLog(SuccessPreset(), "Sound played");
}

void engine::SoundsEngine::StopSound(const std::string& inID)
{
    auto it = m_soundsImpl->m_channels.find(inID);

    if (it != m_soundsImpl->m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->stop();

        if (result != FMOD_OK)
            return;
    }
}

void engine::SoundsEngine::PauseSound(const std::string& inID, bool inIsPaused)
{
    auto it = m_soundsImpl->m_channels.find(inID);

    if (it != m_soundsImpl->m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->setPaused(inIsPaused);

        if (result != FMOD_OK)
            return;
    }
}

void engine::SoundsEngine::SetVolumeSound(const std::string& inID, float inVolume)
{
    auto it = m_soundsImpl->m_channels.find(inID);

    if (inVolume < 0.f)
    {
        inVolume = 0.f;
    }
    else if (inVolume > 1.f)
    {
        inVolume = 1.f;
    }

    if (it != m_soundsImpl->m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->setVolume(inVolume);

        if (result != FMOD_OK)
            return;
    }
}

void engine::SoundsEngine::CloseSoundsEngine(void)
{
    for (auto& [id, sound] : m_soundsImpl->m_sounds)
    {
        if (sound)
        {
            FMOD_RESULT result = sound->release();

            if (result != FMOD_OK)
                continue;
        }
    }
    m_soundsImpl->m_sounds.clear();
    m_soundsImpl->m_channels.clear();

    if (m_soundsImpl->m_system)
    {
        FMOD_RESULT result = m_soundsImpl->m_system->close();

        if (result != FMOD_OK)
            return;
    }

    m_soundsImpl->m_system = nullptr;
}