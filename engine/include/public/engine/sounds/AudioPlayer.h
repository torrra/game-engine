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
    struct Listener
    {
        math::Vector3f m_position   = math::Vector3f::Zero();
        math::Vector3f m_forward    = math::Vector3f::Zero();
        math::Vector3f m_up         = math::Vector3f::Zero();
        math::Vector3f m_velocity   = math::Vector3f::Zero();
    };

    class AudioPlayer : public Component
    {
    public :

        /// Constructor
        // Component constructor
        ENGINE_API          
                            AudioPlayer(EntityHandle inOwner, class SceneGraph* inScene);
        // Move constructor
        ENGINE_API          
                            AudioPlayer(AudioPlayer&& inOther) noexcept = default;

        /// Destructor
        ENGINE_API
                            ~AudioPlayer(void) override = default;

        /// Operator
        ENGINE_API
            AudioPlayer&    operator=(AudioPlayer&& inOther) noexcept = default;

        /// Getters
        /*
            Get the position of the sound
            <return> [out] Vector3f : Position of the sound
        */
        ENGINE_API [[nodiscard]]
            math::Vector3f  GetSoundPosition(void);
        /*
            Get the velocity of the sound
            <return> [out] Vector3f : Velocity of the sound
        */
        ENGINE_API [[nodiscard]]
            math::Vector3f  GetSoundVelocity(void);
        /*
            Get the volume of the sound
            <return> [out] f32 : Volume of the sound
        */
        ENGINE_API [[nodiscard]]
            f32             GetSoundVolume(void);

        /*
            Get the sound
            <return> [out] Sound : The sound
        */
        ENGINE_API [[nodiscard]]
        const Sound*        GetSound(void) const;

        ENGINE_API [[nodiscard]]
            bool            GetIs3DSound(void) const;

        ENGINE_API [[nodiscard]]
            bool            GetIsPlayed(void) const;

        ENGINE_API [[nodiscard]]
            math::Vector3f GetListenerPosition(void) const;
        ENGINE_API [[nodiscard]]
            math::Vector3f GetListenerForward(void) const;
        ENGINE_API [[nodiscard]]
            math::Vector3f GetListenerUp(void) const;
        ENGINE_API [[nodiscard]]
            math::Vector3f GetListenerVelocity(void) const;

        /// Setters
        /*
            Set the volume of a sound
            <param> [in] inID       : ID of the sound
            <param> [in] inVolume   : Volume of the sound : 0.0f to 1.0f
        */
        ENGINE_API 
            void            SetVolumeSound(f32 inVolume = 1.f);
        /*
            Set the position of the sound
            <param> [in] inPosition : Vector3f : The position of the sound
        */
        ENGINE_API
            void            SetSoundPosition(const math::Vector3f& inPosition);
        /*
            Set the velocity of the sound
            <param> [in] inVelocity : Vector3f : The velocity of the sound
        */
        ENGINE_API
            void            SetSoundVelocity(const math::Vector3f& inVelocity);
        /*
            Set the 3D position of a sound
            <param> [in] inPosition : Position of the sound
            <param> [in] inForward  : Forward vector of the sound
            <param> [in] inUp       : Up vector of the sound
            <param> [in] inVelocity : Take the velocity of the listener (optional)
        */
        ENGINE_API 
            void            SetListener(const math::Vector3f& inPosition,
                                        const math::Vector3f& inForward,
                                        const math::Vector3f& inUp,
                                        const math::Vector3f& inVelocity =
                                                            math::Vector3f::Zero());

        ENGINE_API
            void            SetListenerPosition(const math::Vector3f& inPosition);
        ENGINE_API
            void            SetListenerForward(const math::Vector3f& inForward);
        ENGINE_API
            void            SetListenerUp(const math::Vector3f& inUp);
        ENGINE_API
            void            SetListenerVelocity(const math::Vector3f& inVelocity);

        ENGINE_API
            void            SetIs3DSound(bool inIs3DSound);
        ENGINE_API
            void            SetIsPlayed(bool inIsPlayed);

        ENGINE_API 
            void            SetSound(const char* inSoundName);
        ENGINE_API 
            void            SetSound(const Sound* inSound);

        /// Functions
        ENGINE_API
            void            PlaySound(bool is3DSound = false);
        
        /*
            Stop a sound
            <param> [in] inID : ID of the sound
        */
        ENGINE_API 
            void            StopSound(void);
        /*
            Put a sound in pause or resume
            <param> [in] inID       : ID of the sound
            <param> [in] inIsPaused : Pause flag
        */
        ENGINE_API 
            void            PauseSound(bool inIsPaused);
        ENGINE_API 
            void            Register(void) override {}
        ENGINE_API 
            void            Unregister(void) override {}
        ENGINE_API
            void            Invalidate(void) override;
        ENGINE_API
            void            SerializeText(std::ostream& output, EntityHandle owner, 
                                          uint64 index) const override;
        ENGINE_API
            const char*     DeserializeText(const char* text, const char* end) override;

    private :

        /// Constructor
                            AudioPlayer(void) = delete;
                            AudioPlayer(const AudioPlayer& inOther) = delete;
        /// Operator
            AudioPlayer&    operator=(const AudioPlayer& inOther) = delete;

        /// Functions
        // Play sound without 3D 
            void            PlaySoundWithout3D(void);
        // Play a sound with 3D parameters
            void            PlaySound3D(void);

            void            Serialize3DSound(std::ostream& output) const;
            const char*     Deserialize3DSound(const char* text, const char* end);

        const Sound*    m_sound     = nullptr;
        Listener*       m_listener  = nullptr;
        math::Vector3f  m_position  = math::Vector3f::Zero();
        math::Vector3f  m_velocity  = math::Vector3f::Zero();
        f32             m_volume    = 1.f;
        bool            m_is3DSound = false;
        bool            m_isPlayed  = false;

    }; // !Class AudioPlayer

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<AudioPlayer>()
    {
        return AUDIO_PLAYER;
    }

} // !Namespace engine