#include "engine/sounds/SoundsEngine.h"

#pragma region Internal

#include "sounds/InternalSoundsStruct.hpp"
#include "sounds/InternalFmodConversion.hpp"

#pragma endregion

#include "engine/ConsoleLog.hpp"

engine::SoundEngine* engine::SoundEngine::m_instance = nullptr;

engine::SoundEngine::SoundEngine(void)
{
    m_soundImpl = new SoundImpl();
}

engine::SoundEngine::~SoundEngine(void)
{
    delete m_soundImpl;
    m_soundImpl = nullptr;
}

engine::SoundEngine& engine::SoundEngine::Get(void)
{
    if (m_instance == nullptr)
        m_instance = new SoundEngine();

    return *m_instance;
}

engine::SoundImpl& engine::SoundEngine::GetSoundImpl(void) const
{
    return *m_soundImpl;
}

bool engine::SoundEngine::InitSoundEngine(void)
{
    FMOD_RESULT result = FMOD::System_Create(&m_soundImpl->m_system);
    if (result != FMOD_OK)
        return false;

    result = m_soundImpl->m_system->init(512, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK)
        return false;

    return true;
}

void engine::SoundEngine::UpdateSoundEngine(void)
{
    if (!m_soundImpl->m_system)
        return;

    m_soundImpl->m_system->update();
}

void engine::SoundEngine::CloseSoundEngine(void)
{
    m_soundImpl->m_channels.clear();

    if (m_soundImpl->m_system)
    {
        FMOD_RESULT result = m_soundImpl->m_system->close();

        if (result != FMOD_OK)
            return;
    }

    m_soundImpl->m_system = nullptr;

    delete m_instance;
    m_instance = nullptr;

    PrintLog(SuccessPreset(), "Sound engine closed.");
}