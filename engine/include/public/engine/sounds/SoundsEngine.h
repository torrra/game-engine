#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Standard

#include <string>
#include <unordered_map>

#pragma endregion

namespace FMOD
{
    class System;
    class Sound;
    class Channel;

} // !Namespace FMOD

namespace engine
{
    struct ConsoleLog;

    class SoundsEngine
    {
    private :

        using SoundsMap = std::unordered_map<std::string, FMOD::Sound*>;
        using ChannelsMap = std::unordered_map<std::string, FMOD::Channel*>;

    public :

        bool m_isPlaying = false;
        bool m_isPaused = false;

        /// Constructor
        ENGINE_API      SoundsEngine(void);

        /// Destructor
        ENGINE_API      ~SoundsEngine(void);

        /// Functions
        ENGINE_API bool InitSoundsEngine(void);
        ENGINE_API void UpdateSoundsEngine(void);
        ENGINE_API void CloseSoundsEngine(void);

        ENGINE_API bool LoadSound(const std::string& inID, const std::string& inPath, 
                                  bool inLoop = false);
        ENGINE_API void PlaySound(const std::string& inID);
        ENGINE_API void StopSound(const std::string& inID);
        ENGINE_API void PauseSound(const std::string& inID, bool inIsPaused);
        ENGINE_API void SetVolumeSound(const std::string& inID, float inVolume);

    private :

        /// Functions
        ConsoleLog& SoundsEngineErrorLog(void);
        ConsoleLog& SoundsEngineWarningLog(void);
        ConsoleLog& SoundsEngineInfoLog(void);
        ConsoleLog& SoundsEngineSuccessLog(void);

        /// Private members
        SoundsMap       m_sounds;
        ChannelsMap     m_channels;
        ConsoleLog*     m_log       = nullptr;
        FMOD::System*   m_system    = nullptr;

    }; // !Class SoundsEngine
} // !Namespace engine