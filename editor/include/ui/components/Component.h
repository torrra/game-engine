#pragma once

#include <engine/CoreTypes.h>
#include <engine/core/Component.h>

#include <concepts>
#include <string>

namespace editor
{
    template<class TComponentType>
    concept CValidComponentType = std::derived_from<TComponentType, engine::Component>;

    enum EComponentType
    {
        INVALID_COMPONENT_TYPE,
        AUDIO,
        CAMERA,
        RENDERER,
        RIGIDBODY,
        SCRIPT,
        TRANSFORM
    };

    class BaseComponent
    {
    public:
        void RenderSection(engine::SceneGraph* graph, engine::EntityHandle const& handle);
    
        template<CValidComponentType ComponentType>
        void SetData(ComponentType* component);

        bool IsRemoved(void) const noexcept;

    protected:
        virtual void SectionContent(void);
        bool InputField(const char* uid, f32* value, f32 increment, f32 width = 60.0f, f32 xOffset = 0.0f);
        void SetName(const char* name);
        void SetType(EComponentType type);

        template <CValidComponentType ComponentType>
        ComponentType* GetData(void); 
        
    private:
        void* m_component = nullptr;
        std::string m_componentName;
        EComponentType m_componentType = INVALID_COMPONENT_TYPE;
    
    protected:
        bool m_isClosed = false;
    };

    template<CValidComponentType ComponentType>
    inline void BaseComponent::SetData(ComponentType* component)
    {
        m_component = component;
    }

    template<CValidComponentType ComponentType>
    inline ComponentType* BaseComponent::GetData(void)
    {
        return reinterpret_cast<ComponentType*>(m_component);
    }
}