#pragma once

#include "ui/components/Component.h"
#include <engine/ui/UIWindow.h>
#include <engine/core/SceneGraph.h>
#include <engine/utility/MemoryCheck.h>

#include <vector>
#include <concepts>

namespace editor
{


    class PropertyWnd : public ::ui::UIWindow
    {
    public:
        // Should not use this constructor, is required for STL container
        PropertyWnd(void) = default;
        PropertyWnd(engine::SceneGraph* graph);
        ~PropertyWnd(void);

        void SetHandle(engine::EntityHandle handle);
        engine::EntityHandle GetHandle(void) const noexcept;
        void ClearComponentArray(void);

    protected:
        virtual void RenderContents(void) override;
        
    private:
        void InitComponents(void);

        template<CComponentUIType CUIType, CComponentType CType>
        void InitComponent(void);

        template<CComponentUIType CUIType, CComponentType CType>
        void AddComponent(void);

        // Menu bar
        void RenderMenuBar(void);

        // Allow to drag and drop scripts into window
        void ScriptInput(void);
        void AddScript(engine::Script* script, const std::string& name);

        std::vector<BaseComponent*> m_components;
        engine::SceneGraph* m_graph;
        engine::EntityHandle m_handle;
    };

    template<CComponentUIType CUIType, CComponentType CType>
    inline void editor::PropertyWnd::InitComponent(void)
    {
        CUIType* component = new CUIType;
        dynamic_cast<BaseComponent*>(component)->SetData(m_graph->GetComponent<CType>(m_handle));
        m_components.emplace_back(component);
    }

    template<CComponentUIType CUIType, CComponentType TComponent>
    inline void editor::PropertyWnd::AddComponent(void)
    {
        // Prevent duplicate components or bad things happen...
        if (m_graph->GetComponent<TComponent>(m_handle))
            return;

        m_graph->CreateComponent<TComponent>(m_handle);
        InitComponent<CUIType, TComponent>();
    }
}