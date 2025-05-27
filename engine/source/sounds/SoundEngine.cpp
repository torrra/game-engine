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

    // We can store the graph in a local variable for
    // readability
    SceneGraph* graph = Engine::GetEngine()->GetGraph();

    // The type used in a ranged for-loop is actually AudioPlayer,
    // not an iterator
    for (AudioPlayer& it : graph->GetComponentArray<AudioPlayer>())
    {
        // This check removes the need to check if the entity
        // pointer is valid, and makes sure a deactivated
        // component is not updated
        if (!it.IsValid() || !it.IsActive())
            continue;

        // We can move on to the next component
        // if this one doesn't have a sound
        if (it.GetSound() == nullptr)
            continue;

        // Instead of checking if the entity has a transform, and then get the
        // transform, we can check if the transform is non-null to achieve
        // the same result
        if (Transform* transform = graph->GetComponent<Transform>(it.GetOwner()))
            it.SetSoundPosition(transform->GetPosition());
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
