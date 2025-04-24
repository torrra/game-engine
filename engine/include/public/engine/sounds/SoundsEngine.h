#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Standard

#include <string>

#pragma endregion

namespace engine
{
    struct SoundsImpl;

    class SoundsEngine
    {
    public :

        bool m_isPlaying = false;
        bool m_isPaused = false;
        bool m_isPressed = false;

        /// Constructor
        ENGINE_API              SoundsEngine(void);

        /// Destructor
        ENGINE_API              ~SoundsEngine(void);

        /// Functions
        ENGINE_API bool         InitSoundsEngine(void);
        ENGINE_API void         UpdateSoundsEngine(void);
        ENGINE_API void         CloseSoundsEngine(void);

        ENGINE_API bool         LoadSound(const std::string& inID, const std::string& inPath, 
                                          bool inLoop = false);
        ENGINE_API void         PlaySound(const std::string& inID);
        ENGINE_API void         StopSound(const std::string& inID);
        ENGINE_API void         PauseSound(const std::string& inID, bool inIsPaused);
        ENGINE_API void         SetVolumeSound(const std::string& inID, float inVolume);

    private :

        /// Getter
        SoundsImpl*  GetSoundsImpl(void) const;

        /// Private members
        SoundsImpl*     m_soundsImpl    = nullptr;

    }; // !Class SoundsEngine
} // !Namespace engine