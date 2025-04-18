#pragma once

#include <engine/CoreTypes.h>

#include <string>

namespace editor
{
    class BaseComponent
    {
    public:
        void RenderSection(void);
    
    protected:
        virtual void SectionContent(void);
        void SetName(const char* name);

        bool InputField(const char* uid, f32* value, f32 increment, f32 width = 60.0f, f32 xOffset = 0.0f);
    private:
        std::string m_componentName;
    };
}