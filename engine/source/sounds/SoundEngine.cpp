#include "engine/sounds/SoundEngine.h"

#pragma region Internal

#include "sounds/InternalSoundsStruct.hpp"
#include "sounds/InternalFmodConversion.hpp"

#pragma endregion

#pragma region Engine

#include "engine/Engine.h"

#pragma endregion

#pragma region Sound

#include "engine/sounds/AudioPlayer.h"

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

    for (auto& it : Engine::GetEngine()->GetCurrentScene().GetGraph()->GetComponentArray<AudioPlayer>())
    {
        if (Engine::GetEngine()->GetCurrentScene().GetGraph() != nullptr)
        {
            if (Engine::GetEngine()->GetCurrentScene().GetGraph()->GetEntity(it.GetOwner()) != nullptr)
            {
                if (Engine::GetEngine()->GetCurrentScene().GetGraph()->GetEntity(it.GetOwner())->HasComponent<Transform>())
                {
                    if (it.GetSound() != nullptr)
                    {
                        it.SetSoundPosition(Engine::GetEngine()->GetCurrentScene().GetGraph()->GetComponent<Transform>(
                            it.GetOwner())->GetPosition());
                    }
                }
            }
        }
    }
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

void engine::SoundEngine::StopSounds(void)
{
    for (auto& sound : Engine::GetEngine()->GetGraph()->GetComponentArray<AudioPlayer>())
        sound.StopSound();
}
