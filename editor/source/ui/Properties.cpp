#include "ui/Properties.h"
#include "ui/components/CameraComponent.h"
#include "ui/components/RendererComponent.h"
#include "ui/components/ScriptComponent.h"
#include "ui/components/TransformComponent.h"

#include <engine/core/SceneGraph.h>
#include <engine/utility/MemoryCheck.h>

#define WINDOW_NAME "Properties"
#define INVALID_HANDLE engine::Entity::EHandleUtils::INVALID_HANDLE

editor::PropertyWnd::PropertyWnd(void)
    : PropertyWnd(nullptr)
{
}

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
    {
        delete component;
    }

    m_components.clear();
    
    if (m_handle == INVALID_HANDLE)
        return;

    engine::Entity* entity = m_graph->GetEntity(m_handle);

    // Transform
    if (entity->HasComponent<engine::Transform>())
    {
        TransformComponent* transformComponent = new TransformComponent;
        transformComponent->SetTransform(m_graph->GetComponent<engine::Transform>(m_handle));
        m_components.emplace_back(transformComponent);
    }

    // Renderer
    if (entity->HasComponent<engine::Renderer>())
        m_components.emplace_back(new RendererComponent);
    
    // Camera
    if (entity->HasComponent<engine::Camera>())
        m_components.emplace_back(new CameraComponent);
    
    // Script
    if (entity->HasComponent<engine::Script>())
        m_components.emplace_back(new ScriptComponent);
}
