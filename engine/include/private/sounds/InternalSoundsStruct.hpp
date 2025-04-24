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
    using SoundsMap = std::unordered_map<std::string, FMOD::Sound*>;
    using ChannelsMap = std::unordered_map<std::string, FMOD::Channel*>;

    struct SoundsImpl
    {
        SoundsMap       m_sounds;
        ChannelsMap     m_channels;
        FMOD::System*   m_system    = nullptr;

    }; // !Struct SoundsImpl
} // !Namespace engine