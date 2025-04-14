#pragma once

#include <string>

namespace editor
{
    class ComponentProperty
    {
    public:
        void RenderSection(void);
    
    protected:
        virtual void SectionContent(void);
        void SetName(const char* name);

    private:
        std::string m_componentName;
    };
}