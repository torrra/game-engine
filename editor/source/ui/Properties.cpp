#include "ui/Properties.h"
#include "ui/components/CameraComponent.h"
#include "ui/components/RendererComponent.h"
#include "ui/components/ScriptComponent.h"
#include "ui/components/TransformComponent.h"

#include <engine/core/SceneGraph.h>
#include <engine/utility/MemoryCheck.h>

#define WINDOW_NAME "Properties"
#define INVALID_HANDLE engine::Entity::EHandleUtils::INVALID_HANDLE

editor::PropertyWnd::PropertyWnd(engine::SceneGraph* graph)
{
    SetName(WINDOW_NAME);

    m_handle = INVALID_HANDLE;
    m_graph = graph;
}

editor::PropertyWnd::~PropertyWnd(void)
{
    for (BaseComponent* component : m_components)
    {
        delete component;
    }

    m_components.clear();

    m_handle = INVALID_HANDLE;
    m_graph = nullptr;
}

void editor::PropertyWnd::SetHandle(engine::EntityHandle handle)
{
    m_handle = handle;

    InitComponents();
}

void editor::PropertyWnd::RenderContents(void)
{
    if (m_handle == INVALID_HANDLE)
        return;

    // Render all components
    for (BaseComponent* component : m_components)
        component->RenderSection();

}

void editor::PropertyWnd::InitComponents(void)
{
    for (BaseComponent* component : m_components)
        delete component;
    m_components.clear();
    
    if (m_handle == INVALID_HANDLE)
        return;

    engine::Entity* entity = m_graph->GetEntity(m_handle);

    // Transform
    if (entity->HasComponent<engine::Transform>())
        InitComponent<TransformComponent, engine::Transform>();

    // Renderer
    if (entity->HasComponent<engine::Renderer>())
        InitComponent<RendererComponent, engine::Renderer>();
    
    // Camera
    if (entity->HasComponent<engine::Camera>())
        InitComponent<CameraComponent, engine::Camera>();
    
    // Script
    if (entity->HasComponent<engine::Script>())
    {
        engine::Script* scriptComponent = m_graph->GetComponent<engine::Script>(m_handle);

        for (engine::ScriptObject script : scriptComponent->GetScripts())
            m_components.emplace_back(new ScriptComponent(script.GetType()));
    }
}
