#pragma once

#include "ui/components/Component.h"
#include <engine/scripting/ScriptObject.h>

#include <string>

namespace editor
{
    class ScriptComponent : public BaseComponent
    {
    public:
        ScriptComponent(void) = delete;
        ScriptComponent(std::string const& scriptName);
        ~ScriptComponent(void);

        void SetScript(engine::ScriptObject* script);
    
    protected:
        virtual void SectionContent(void) override;

    private:
        engine::ScriptObject* m_script;
    };
}