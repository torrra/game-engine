#pragma once

#include <vector>

#include "engine/core/Component.h"
#include "engine/core/Entity.h"

#include "engine/scripting/ScriptObject.h"

namespace engine
{
    class Script : public Component
    {
    public:

        using Component::Component;
        
        // Use parent class constructors
        ENGINE_API
        Script(void) = default;

        ENGINE_API
        Script(const Script&) = delete;

        ENGINE_API
        Script(Script&&) noexcept = default;
        
        ENGINE_API
        ~Script(void) = default;

        // Call Start() in all script objects

        ENGINE_API
        void Start(void);

        // Tick all script objects
        ENGINE_API
        void Update(f32 deltaTime);

        // Register script component and save a reference in lua
        ENGINE_API
        void Register(void) override;

        ENGINE_API
        void Unregister(void) override;

        // Add a new lua script object to this component, and register it
        // in lua
        ENGINE_API
        void AddScriptObject(const std::string& type);

        ENGINE_API
        void SerializeText(std::ostream& output,
                           EntityHandle owner,
                           uint64 index) const override;

        ENGINE_API
        const char* DeserializeText(const char* text, const char* end) override;

        ENGINE_API
        Script& operator=(const Script&) = default;

        ENGINE_API
        Script& operator=(Script&&) noexcept = default;

        ENGINE_API
        const std::vector<ScriptObject>& GetScripts(void) const;

    private:

        std::vector<ScriptObject> m_scriptObjects;
        
    };

    template <>
    struct UpdateAfterParent<Script>
    {
        static constexpr bool m_value = true;
    };

    template <>
    struct UpdateComponent<Script>
    {
        static constexpr bool m_value = true;
    };

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<Script>()
    {
        return SCRIPT;
    }

}