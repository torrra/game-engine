#include "ui/Properties.h"

#include <engine/core/SceneGraph.h>
#include <engine/utility/MemoryCheck.h>

#define WINDOW_NAME "Properties"
#define INVALID_HANDLE engine::Entity::EHandleUtils::INVALID_HANDLE

editor::PropertyWnd::PropertyWnd(void)
{
    SetName(WINDOW_NAME);

    m_handle = INVALID_HANDLE;
    m_graph = nullptr;

    m_transform = new TransformComponent;
    m_camera = new CameraComponent;
    m_renderer = new RendererComponent;
    m_script = new ScriptComponent;
}

editor::PropertyWnd::PropertyWnd(engine::SceneGraph* graph)
{
    SetName(WINDOW_NAME);

    m_handle = INVALID_HANDLE;
    m_graph = graph;

    m_transform = new TransformComponent;
    m_camera = new CameraComponent;
    m_renderer = new RendererComponent;
    m_script = new ScriptComponent;
}

editor::PropertyWnd::~PropertyWnd(void)
{
    delete m_transform;
    delete m_camera;
    delete m_renderer;
    delete m_script;

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
    m_components.clear();
    
    if (m_handle == INVALID_HANDLE)
        return;

    engine::Entity* entity = m_graph->GetEntity(m_handle);

    // Transform
    if (entity->HasComponent<engine::Transform>())
    {
        m_transform->SetTransform(m_graph->GetComponent<engine::Transform>(m_handle));
        m_components.emplace_back(m_transform);
    }

    // Renderer
    if (entity->HasComponent<engine::Renderer>())
        m_components.emplace_back(m_renderer);
    
    // Camera
    if (entity->HasComponent<engine::Camera>())
        m_components.emplace_back(m_camera);
    
    // Script
    if (entity->HasComponent<engine::Script>())
        m_components.emplace_back(m_script);
}
