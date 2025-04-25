#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Standard

#include <string>

#pragma endregion

#pragma region Math

#include <math/Vector3.hpp>

#pragma endregion

#pragma region Core

#include "engine/core/Component.h"

#pragma endregion

namespace engine
{
    struct SoundsImpl;

    class SoundsEngine : public Component
    {
    public :

        /// Constructor
        // Default constructor to initialize the struct implementation
                            SoundsEngine(void) = delete;
        ENGINE_API          SoundsEngine(EntityHandle inOwner, class SceneGraph* inScene);


        /// Destructor
        // Default destructor to delete the struct implementation
        ENGINE_API          ~SoundsEngine(void);

        /// Setters
        /*
            Set the volume of a sound
            <param> [in] inID       : ID of the sound
            <param> [in] inVolume   : Volume of the sound : 0.0f to 1.0f
        */
        ENGINE_API void     SetVolumeSound(const std::string& inID, f32 inVolume = 1.f);
        /*
            Set the 3D position of a sound
            <param> [in] inPosition : Position of the sound
            <param> [in] inForward  : Forward vector of the sound
            <param> [in] inUp       : Up vector of the sound
            <param> [in] inVelocity : Take the velocity of the listener (optional)
        */
        ENGINE_API void     SetListenerPosition(const math::Vector3f& inPosition,
                                                const math::Vector3f& inForward,
                                                const math::Vector3f& inUp,
                                                const math::Vector3f& inVelocity = 
                                                            math::Vector3f::Zero());

        /// Functions
        /*
            Initialize the sounds engine system
            <param> default max channel set to 512
            <param> default init flag set to FMOD_INIT_NORMAL
        */
        ENGINE_API bool     InitSoundsEngine(void);
        // Update the sounds engine system
        ENGINE_API void     UpdateSoundsEngine(void);
        // Close the sounds engine system, channels and sounds
        ENGINE_API void     CloseSoundsEngine(void);
        /*
            Load a sound
            <param> [in] inID   : ID of the sound
            <param> [in] inPath : Path of the sound
            <param> [in] inLoop : Loop flag : set by default to false (optional)
        */
        ENGINE_API bool     LoadSound(const std::string& inID, const std::string& inPath, 
                                      bool inLoop = false);
        /*
            Play a sound
            <param> [in] inID       : ID of the sound
            <param> [in] inVolume   : Volume of the sound   : 0.0f to 1.0f (optional)
            <param> [in] inPaused   : Pause flag (optional)
        */
        ENGINE_API void     PlaySound(const std::string& inID, f32 inVolume = 1.f,
                                      bool inPaused = false);
        /*
            Play a 3D sound
            <param> [in] inID       : ID of the sound
            <param> [in] inPosition : Position of the sound
            <param> [in] inVelocity : Velocity of the sound (Calculate the doppler effect) (optional)
            <param> [in] inVolume   : Volume of the sound   : 0.0f to 1.0f (optional)
        */
        ENGINE_API void     PlaySound3D(const std::string& inID, 
                                        const math::Vector3f& inPosition,
                                        const math::Vector3f& inVelocity = math::Vector3f::Zero(),
                                        f32 inVolume = 1.f);
        /*
            Stop a sound
            <param> [in] inID : ID of the sound
        */
        ENGINE_API void     StopSound(const std::string& inID);
        /*
            Put a sound in pause or resume
            <param> [in] inID       : ID of the sound
            <param> [in] inIsPaused : Pause flag
        */
        ENGINE_API void     PauseSound(const std::string& inID, bool inIsPaused);
        ENGINE_API void     Register(void) override {}

    private :

        /// Getter
        // Get the struct implementation
        SoundsImpl*         GetSoundsImpl(void) const;

        /// Private members
        SoundsImpl* m_soundsImpl    = nullptr;

    }; // !Class SoundsEngine
} // !Namespace engine