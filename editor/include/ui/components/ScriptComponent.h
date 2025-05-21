#pragma once

#include "ui/components/Component.h"
#include <engine/scripting/ScriptObject.h>

#include <string>

namespace editor
{
    class ScriptComponent final : public BaseComponent
    {
    public:
        ScriptComponent(void) = delete;
        ScriptComponent(std::string const& scriptName);
        ScriptComponent(const ScriptComponent&) = delete;
        ScriptComponent(ScriptComponent&&) noexcept = default;
        ~ScriptComponent(void) = default;

        void SetScript(uint32 script);
    
    protected:
        void SectionContent(void) override;
        void RenderSection(engine::SceneGraph* graph,
                           engine::EntityHandle const& handle) override;

    private:
        uint32 m_script = ULONG_MAX;
    };
}