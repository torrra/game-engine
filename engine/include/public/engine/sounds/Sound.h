#pragma once

#pragma region Resources

#include "engine/resource/Resource.h"

#pragma endregion

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Standard

#include <string>

#pragma endregion

namespace engine
{
    class Sound final : public IResource
    {
    public :

        /// Constructor
        ENGINE_API              Sound(void) = default;

        /// Destructor
        ENGINE_API              ~Sound(void) = default;

        /// Getters
        ENGINE_API bool         GetIsLooping(void) const;
        ENGINE_API std::string  GetID(void) const;
        ENGINE_API void*        GetSound(void) const;

        /// Setters
        ENGINE_API void         SetLooping(bool inIsLooping);

        /// Functions
        ENGINE_API bool         LoadResource(const char* inPath) override;

    private :

        void*       m_sound     = nullptr;
        std::string m_ID        = "";
        bool        m_isLooping = false;

    }; // !Class Sound
} // !Namespace engine