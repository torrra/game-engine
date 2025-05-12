#pragma once

#pragma region FMOD

#include <fmod/fmod.hpp>

#pragma endregion

#pragma region Standard

#include <string>
#include <unordered_map>

#pragma endregion

namespace engine
{
    using ChannelsMap   = std::unordered_map<std::string, FMOD::Channel*>;

    struct SoundImpl
    {
        ChannelsMap     m_channels;
        FMOD::System*   m_system    = nullptr;

    }; // !Struct SoundsImpl
} // !Namespace engine