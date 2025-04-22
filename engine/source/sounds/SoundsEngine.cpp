#include "engine/sounds/SoundsEngine.h"

#pragma region Engine

#include "engine/ConsoleLog.hpp"

#pragma endregion

#include <fmod/fmod.hpp>

engine::SoundsEngine::SoundsEngine(void)
{
    m_log = new ConsoleLog();
}

engine::SoundsEngine::~SoundsEngine(void)
{
    delete m_log;
    m_log = nullptr;

    m_system->release();
    m_system = nullptr;
}

bool engine::SoundsEngine::InitSoundsEngine(void)
{
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    if (result != FMOD_OK)
    {
        m_log = &SoundsEngineErrorLog();
        PrintLog(*m_log, "Unable to create FMOD system");
        return false;
    }

    m_log = &SoundsEngineInfoLog();
    PrintLog(*m_log, "FMOD system created");

    result = m_system->init(512, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK)
    {
        m_log = &SoundsEngineErrorLog();
        PrintLog(*m_log, "Unable to initialize FMOD");
        return false;
    }

    m_log = &SoundsEngineSuccessLog();
    PrintLog(*m_log, "FMOD initialized");
    return true;
}

void engine::SoundsEngine::UpdateSoundsEngine(void)
{
    if (!m_system)
    {
        m_log = &SoundsEngineErrorLog();
        PrintLog(*m_log, "FMOD system not created");
        return;
    }
    m_system->update();
}

bool engine::SoundsEngine::LoadSound(const std::string& inID, const std::string& inPath, 
                                     bool inLoop)
{
    if (m_sounds.find(inID) != m_sounds.end())
    {
        m_log = &SoundsEngineWarningLog();
        PrintLog(*m_log, "Sound " + inID + " already loaded");
        return true;
    }

    FMOD_MODE mode = FMOD_DEFAULT;
    if (inLoop)
        mode |= FMOD_LOOP_NORMAL;

    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = m_system->createSound(inPath.c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK)
    {
        m_log = &SoundsEngineErrorLog();
        PrintLog(*m_log, "Unable to load sound " + inID);
        return false;
    }

    m_sounds[inID] = sound;

    m_log = &SoundsEngineSuccessLog();
    PrintLog(*m_log, "Sound " + inID + " loaded");

    return true;
}

void engine::SoundsEngine::PlaySound(const std::string& inID)
{
    auto it = m_sounds.find(inID);

    if (it == m_sounds.end())
    {
        m_log = &SoundsEngineWarningLog();
        PrintLog(*m_log, "Sound " + inID + " not loaded");
        return;
    }

    FMOD::Channel* channel = nullptr;
    m_system->playSound(it->second, nullptr, true, &channel);
    if (channel)
    {
        channel->setVolume(1.0f);
        channel->setPaused(false);
        m_channels[inID] = channel;

        m_log = &SoundsEngineSuccessLog();
        PrintLog(*m_log, "Sound " + inID + " played");
    }
}

void engine::SoundsEngine::StopSound(const std::string& inID)
{
    auto it = m_channels.find(inID);

    if (it != m_channels.end() && it->second)
    {
        FMOD_RESULT result = it->second->stop();

        if (result != FMOD_OK)
        {
            m_log = &SoundsEngineErrorLog();
            PrintLog(*m_log, "Unable to stop sound " + inID);
        }
        else
        {
            m_log = &SoundsEngineSuccessLog();
            PrintLog(*m_log, "Sound " + inID + " stopped");
        }
    }
}

void engine::SoundsEngine::PauseSound(const std::string& inID, bool inIsPaused)
{
    auto it = m_channels.find(inID);

    if (it != m_channels.end() && it->second)
    {
        it->second->setPaused(inIsPaused);

        m_log = &SoundsEngineSuccessLog();
        PrintLog(*m_log, "Sound " + inID + " paused");
    }
}

void engine::SoundsEngine::SetVolumeSound(const std::string& inID, float inVolume)
{
    auto it = m_channels.find(inID);

    if (it != m_channels.end() && it->second)
    {
        it->second->setVolume(inVolume);

        m_log = &SoundsEngineSuccessLog();
        PrintLog(*m_log, "Sound " + inID + " volume set to " + std::to_string(inVolume));
    }
}

void engine::SoundsEngine::CloseSoundsEngine(void)
{
    for (auto& [id, sound] : m_sounds)
    {
        if (sound)
        {
            sound->release();

            m_log = &SoundsEngineSuccessLog();
            PrintLog(*m_log, "Sound " + id + " closed");
        }
    }
    m_sounds.clear();
    m_channels.clear();

    if (m_system)
    {
        m_system->close();
        m_log = &SoundsEngineSuccessLog();
        PrintLog(*m_log, "FMOD closed");
    }
}

engine::ConsoleLog& engine::SoundsEngine::SoundsEngineErrorLog(void)
{
    m_log->m_bgColor    = EBGColor::BG_RED;
    m_log->m_fgColor    = EFGColor::FG_WHITE;
    m_log->m_textFormat = EGRP::BLINK;
    m_log->m_prefix     = "[ SoundsEngine ] Error :";

    return *m_log;
}

engine::ConsoleLog& engine::SoundsEngine::SoundsEngineWarningLog(void)
{
    m_log->m_bgColor    = EBGColor::BG_YELLOW;
    m_log->m_fgColor    = EFGColor::FG_BLACK;
    m_log->m_textFormat = EGRP::BLINK;
    m_log->m_prefix     = "[ SoundsEngine ] Warning :";

    return *m_log;
}

engine::ConsoleLog& engine::SoundsEngine::SoundsEngineInfoLog(void)
{
    m_log->m_bgColor    = EBGColor::BG_BLUE;
    m_log->m_fgColor    = EFGColor::FG_WHITE;
    m_log->m_textFormat = EGRP::NORMAL;
    m_log->m_prefix     = "[ SoundsEngine ] Info :";

    return *m_log;
}

engine::ConsoleLog& engine::SoundsEngine::SoundsEngineSuccessLog(void)
{
    m_log->m_bgColor    = EBGColor::BG_GREEN;
    m_log->m_fgColor    = EFGColor::FG_WHITE;
    m_log->m_textFormat = EGRP::NORMAL;
    m_log->m_prefix     = "[ SoundsEngine ] Success :";

    return *m_log;
}
