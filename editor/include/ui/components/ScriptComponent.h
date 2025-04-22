#pragma once

#include "ui/components/Component.h"

#include <engine/CoreTypes.h>

namespace editor
{
    class ScriptComponent : public BaseComponent
    {
    public:
        ScriptComponent(void);
        ~ScriptComponent(void);

    protected:
        virtual void SectionContent(void) override;

    private:
    };
}