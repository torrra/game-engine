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
        // Default constructor to initialize the struct implementation
        ENGINE_API              SoundsEngine(void);

        /// Destructor
        // Default destructor to delete the struct implementation
        ENGINE_API              ~SoundsEngine(void);

        /// Functions
        /*
            Initialize the sounds engine system
            <param> default max channel set to 512
            <param> default init flag set to FMOD_INIT_NORMAL
        */
        ENGINE_API bool         InitSoundsEngine(void);
        // Update the sounds engine system
        ENGINE_API void         UpdateSoundsEngine(void);
        // Close the sounds engine system, channels and sounds
        ENGINE_API void         CloseSoundsEngine(void);
        /*
            Load a sound
            <param> [in] inID : ID of the sound
            <param> [in] inPath : Path of the sound
            <param> [in] inLoop : Loop flag : set by default to false (optional)
        */
        ENGINE_API bool         LoadSound(const std::string& inID, const std::string& inPath, 
                                          bool inLoop = false);
        /*
            Play a sound
            <param> [in] inID : ID of the sound
        */
        ENGINE_API void         PlaySound(const std::string& inID);
        /*
            Stop a sound
            <param> [in] inID : ID of the sound
        */
        ENGINE_API void         StopSound(const std::string& inID);
        /*
            Put a sound in pause or resume
            <param> [in] inID : ID of the sound
            <param> [in] inIsPaused : Pause flag
        */
        ENGINE_API void         PauseSound(const std::string& inID, bool inIsPaused);
        /// TODO : Check max and min volume possible
        /*
            Set the volume of a sound
            <param> [in] inID : ID of the sound
            <param> [in] inVolume : Volume of the sound : 0.0f to 1.0f
        */
        ENGINE_API void         SetVolumeSound(const std::string& inID, float inVolume);

    private :

        /// Getter
        // Get the struct implementation
        SoundsImpl*             GetSoundsImpl(void) const;

        /// Private members
        SoundsImpl* m_soundsImpl    = nullptr;

    }; // !Class SoundsEngine
} // !Namespace engine