#pragma once

#include <engine/CoreTypes.h>
#include <engine/core/Component.h>
#include <engine/core/SceneGraph.h>

#include <concepts>
#include <string>

namespace editor
{
    class BaseComponent;

    template<class TComponentType>
    concept CComponentType = std::derived_from<TComponentType, engine::Component>;

    template<typename TDerrivedType>
    concept CComponentUIType = std::derived_from<TDerrivedType, BaseComponent>;

    enum EComponentType
    {
        INVALID_COMPONENT_TYPE,
        AUDIO,
        CAMERA,
        RENDERER,
        RIGIDBODY_STATIC,
        RIGIDBODY_DYNAMIC,
        SCRIPT,
        TRANSFORM
    };

    class BaseComponent
    {
    public:
        void RenderSection(engine::SceneGraph* graph, engine::EntityHandle const& handle);
        bool IsRemoved(void) const noexcept;
    
        //template<CComponentType CType>


        void SetData(engine::SceneGraph* graph, engine::EntityHandle owner);

        EComponentType GetType(void) const noexcept;


    protected:
        virtual void SectionContent(void);
        bool InputField(const char* uid, f32* value, f32 increment, f32 width = 60.0f, f32 xOffset = 0.0f);
        void SetName(const char* name);
        void SetType(EComponentType type);

        template <CComponentType CType>
        CType* GetData(void);


    private:
        engine::SceneGraph* m_graph = nullptr;
        std::string m_componentName;
        engine::EntityHandle m_owner = engine::Entity::INVALID_HANDLE;
        EComponentType m_componentType = INVALID_COMPONENT_TYPE;
    
    protected:
        bool m_isClosed = false;
    };

   /* template<CComponentType CType>
    inline void BaseComponent::SetData(CType* component)
    {
        m_component = component;
    }*/

    template<CComponentType CType>
    inline CType* BaseComponent::GetData(void)
    {
        return m_graph->GetComponent<CType>(m_owner);
    }
}