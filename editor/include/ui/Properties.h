#pragma once

#include "ui/components/Component.h"
#include <engine/ui/UIWindow.h>
#include <engine/core/SceneGraph.h>
#include <engine/utility/MemoryCheck.h>

#include <vector>
#include <concepts>

namespace editor
{
    template<typename TDerrivedType>
    concept CComponentType = std::derived_from<TDerrivedType, BaseComponent>;

    class PropertyWnd : public ::ui::UIWindow
    {
    public:
        PropertyWnd(void) = delete;
        PropertyWnd(engine::SceneGraph* graph);
        ~PropertyWnd(void);

        void SetHandle(engine::EntityHandle handle);

    protected:
        virtual void RenderContents(void) override;
        
    private:
        void InitComponents(void);

        template<CComponentType TComponentType, CValidComponentType TComponent>
        void InitComponent(void);

        template<CComponentType TComponentType, CValidComponentType TComponent>
        void AddComponent(void);

        // Menu bar
        void RenderMenuBar(void);

        std::vector<BaseComponent*> m_components;
        engine::SceneGraph* m_graph;
        engine::EntityHandle m_handle;
    };

    template<CComponentType TComponentType, CValidComponentType TComponent>
    inline void editor::PropertyWnd::InitComponent(void)
    {
        TComponentType* component = new TComponentType;
        dynamic_cast<BaseComponent*>(component)->SetData(m_graph->GetComponent<TComponent>(m_handle));
        m_components.emplace_back(component);
    }

    template<CComponentType TComponentType, CValidComponentType TComponent>
    inline void editor::PropertyWnd::AddComponent(void)
    {
        // Prevent duplicate components or bad things happen...
        if (m_graph->GetComponent<TComponent>(m_handle))
            return;

        m_graph->CreateComponent<TComponent>(m_handle);
        InitComponent<TComponentType, TComponent>();
    }
}