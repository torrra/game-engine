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

}

