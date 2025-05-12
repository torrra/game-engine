#pragma once

#pragma region Core

#include "engine/core/Component.h"

#pragma endregion

#pragma region Math

#include "math/Vector3.hpp"

#pragma endregion

#pragma region Sounds

#include "engine/sounds/Sound.h"

#pragma endregion

namespace engine
{
    class AudioPlayer : public Component
    {
    public :

        /// Constructor
        // Component constructor
        ENGINE_API          AudioPlayer(EntityHandle inOwner, class SceneGraph* inScene);
        // Move constructor
        ENGINE_API          AudioPlayer(AudioPlayer&& inOther) noexcept = default;

        /// Destructor
                            ~AudioPlayer(void) override = default;

        /// Setters
        /*
            Set the volume of a sound
            <param> [in] inID       : ID of the sound
            <param> [in] inVolume   : Volume of the sound : 0.0f to 1.0f
        */
        ENGINE_API void     SetVolumeSound(f32 inVolume = 1.f);
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

        ENGINE_API void SetSound(const Sound* inSound);

        ENGINE_API
            AudioPlayer&    operator=(AudioPlayer&&) noexcept = default;

        /// Functions
        /*
            Load a sound
            <param> [in] inID   : ID of the sound
            <param> [in] inPath : Path of the sound
            <param> [in] inLoop : Loop flag : set by default to false (optional)
        */
        //ENGINE_API bool     LoadSound(const std::string& inID, const std::string& inPath,
        //                              bool inLoop = false);
        /*
            Play a sound
            <param> [in] inID       : ID of the sound
            <param> [in] inVolume   : Volume of the sound   : 0.0f to 1.0f (optional)
            <param> [in] inPaused   : Pause flag (optional)
        */
        ENGINE_API void     PlaySound(f32 inVolume = 1.f,
                                      bool inPaused = false);
        /*
            Play a 3D sound
            <param> [in] inID       : ID of the sound
            <param> [in] inPosition : Position of the sound
            <param> [in] inVelocity : Velocity of the sound (Calculate the doppler effect) (optional)
            <param> [in] inVolume   : Volume of the sound   : 0.0f to 1.0f (optional)
        */
        ENGINE_API void     PlaySound3D(const math::Vector3f& inPosition,
                                        const math::Vector3f& inVelocity = math::Vector3f::Zero(),
                                        f32 inVolume = 1.f);
        /*
            Stop a sound
            <param> [in] inID : ID of the sound
        */
        ENGINE_API void     StopSound(void);
        /*
            Put a sound in pause or resume
            <param> [in] inID       : ID of the sound
            <param> [in] inIsPaused : Pause flag
        */
        ENGINE_API void     PauseSound(bool inIsPaused);
        ENGINE_API void     Register(void) override {}

    private :

        AudioPlayer(void) = delete;

        const Sound*              m_sound = nullptr;

    }; // !Class AudioPlayer

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<AudioPlayer>()
    {
        return AUDIO_PLAYER;
    }

} // !Namespace engine