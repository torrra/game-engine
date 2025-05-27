#pragma once

#pragma region Resource

#include "engine/core/TypesECS.h"

#pragma endregion

#pragma region FMOD

#include <fmod/fmod.hpp>

#pragma endregion

#pragma region Standard

#include <string>
#include <unordered_map>

#pragma endregion

namespace engine
{
    using ChannelsMap   = std::unordered_map<EntityHandle, FMOD::Channel*>;

    struct SoundImpl
    {
        ChannelsMap     m_channels;
        FMOD::System*   m_system    = nullptr;

    }; // !Struct SoundsImpl
} // !Namespace engine