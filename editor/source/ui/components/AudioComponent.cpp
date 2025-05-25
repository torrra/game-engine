#include "ui/components/AudioComponent.h"

#pragma region UI

#include <ui/Assets.h>

#pragma endregion

#pragma region Engine

#include <engine/Engine.h>

#include <engine/CoreTypes.h>

#include <engine/sounds/Sound.h>
#include <engine/sounds/AudioPlayer.h>

#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDragDrop.h>

#include <engine/resource/ResourceManager.h>

#pragma endregion

#pragma region Math

#include <math/Vector3.hpp>

#pragma endregion

#define NO_SOUND_NAME "None##sound"

editor::AudioComponent::AudioComponent(void)
{
    SetName("AudioComponent");
    SetType(AUDIO);

    m_soundName = NO_SOUND_NAME;
}

editor::AudioComponent::~AudioComponent(void)
{
        }

void editor::AudioComponent::SectionContent(void)
{
    if (engine::AudioPlayer* audioPlayer = GetData<engine::AudioPlayer>())
    {
        m_soundName = (audioPlayer->GetSound()) ? audioPlayer->GetSound()->GetID() : NO_SOUND_NAME;

        ui::Text("Sound: ");
        ui::SameLine(150.f);
        ui::Button(m_soundName.c_str());

        if (ui::StartDragDropTarget())
        {
            DragAndDrop(audioPlayer);
}
        ui::VerticalSpacing();

        if (audioPlayer->GetSound())
        {
            bool is3DSound = audioPlayer->GetIs3DSound();
            if (ui::Checkbox("##3DSound", &is3DSound))
            {
                audioPlayer->SetIs3DSound(is3DSound);
            }
            if (is3DSound)
            {
                Sound3DUI(audioPlayer);
                ListenerUI(audioPlayer);
            }            

            bool isPlayed = audioPlayer->GetIsPlayed();
            if (ui::Checkbox("##IsPlayed", &isPlayed))
            {
                f32 volume = audioPlayer->GetSoundVolume();
                ui::Text("SoundVolume: ");
                InputField("##Volume", &volume, 0.05f);
                audioPlayer->SetVolumeSound(volume);
                ui::VerticalSpacing();

                audioPlayer->SetIsPlayed(isPlayed);
                if (isPlayed)
                {
                    audioPlayer->PlaySound(is3DSound);
                }
                else
                {
                    audioPlayer->StopSound();
                }
            }
        }
}
}

void editor::AudioComponent::DragAndDrop(engine::AudioPlayer* inAudioPlayer)
{
    if (const ui::Payload payload = ui::AcceptPayload(AUDIO_PAYLOAD, 0))
    {
        Asset* payloadData = reinterpret_cast<Asset*>(payload.GetData());

        engine::ResourceManager::Load<engine::Sound>(payloadData->m_path.string());
        m_soundName = payloadData->m_path.string().c_str();
        inAudioPlayer->SetSound(payloadData->m_path.string().c_str());        
    }
    ui::EndDragDropTarget();
    }

void editor::AudioComponent::Sound3DUI(engine::AudioPlayer* inAudioPlayer)
{
    f32 volume = inAudioPlayer->GetSoundVolume();
    ui::Text("SoundVolume: ");
    InputField("##Volume", &volume, 0.05f);
    inAudioPlayer->SetVolumeSound(volume);
    ui::VerticalSpacing();

    math::Vector3f position = inAudioPlayer->GetSoundPosition();
    ui::Text("SoundPosition: ");
    InputField("##XPos", &position[0], 0.05f);
    InputField("##YPos", &position[1], 0.05f);
    InputField("##ZPos", &position[2], 0.05f);
    inAudioPlayer->SetSoundPosition(position);
    ui::VerticalSpacing();

    math::Vector3f velocity = inAudioPlayer->GetSoundVelocity();
    ui::Text("SoundVelocity: ");
    InputField("##XVel", &velocity[0], 0.05f);
    InputField("##YVel", &velocity[1], 0.05f);
    InputField("##ZVel", &velocity[2], 0.05f);
    inAudioPlayer->SetSoundVelocity(velocity);
    ui::VerticalSpacing();
}