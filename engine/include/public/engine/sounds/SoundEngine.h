#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

namespace engine
{
    struct SoundImpl;
    class AudioPlayer;

    class SoundEngine
    {
    public :      

        /// Getters
        // Get the instance of the sound engine
        ENGINE_API static   SoundEngine& Get(void);
        // Get the struct implementation
        ENGINE_API          SoundImpl& GetSoundImpl(void) const;

        /// Functions
        /*
            Initialize the sounds engine system
            <param> default max channel set to 512
            <param> default init flag set to FMOD_INIT_NORMAL
        */
        ENGINE_API bool     InitSoundEngine(void);
        // Update the sounds engine system
        ENGINE_API void     UpdateSoundEngine(void);
        // Close the sounds engine system, channels and sounds
        ENGINE_API void     CloseSoundEngine(void);
        // Stop all sounds
        ENGINE_API
            void            StopSounds(void);

    private :

        /// Constructor
        // Default constructor to initialize the struct implementation
                            SoundEngine(void);

        /// Destructor
        // Default destructor to delete the struct implementation
                            ~SoundEngine(void);

        /// Private members
        static  SoundEngine*    m_instance;
                SoundImpl*      m_soundImpl = nullptr;
                AudioPlayer*    m_audioPlayer  = nullptr;

    }; // !Class SoundsEngine
} // !Namespace engine