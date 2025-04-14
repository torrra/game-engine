#include "ui/Properties.h"
#include "ui/components/Transform.h"

#include <engine/ui/UIComponent.h>
#include <engine/core/SceneGraph.h>

#define WINDOW_NAME "Properties"
#define INVALID_HANDLE engine::Entity::EHandleUtils::INVALID_HANDLE

editor::PropertyWnd::PropertyWnd(void)
{
    SetName(WINDOW_NAME);

    m_transform = new TransformProperty();
    m_handle = INVALID_HANDLE;
    m_graph = nullptr;
}

editor::PropertyWnd::PropertyWnd(engine::SceneGraph* graph)
{
    SetName(WINDOW_NAME);

    m_transform = new TransformProperty();
    m_handle = INVALID_HANDLE;
    m_graph = graph;
}

editor::PropertyWnd::~PropertyWnd(void)
{
    if (m_transform)
        delete m_transform;

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

    if (m_hasTransform)
        m_transform->RenderSection();
}

void editor::PropertyWnd::InitComponents(void)
{
    if (m_handle == INVALID_HANDLE)
    {
        m_hasTransform = false;
        return;
    }
    engine::Entity* entity = m_graph->GetEntity(m_handle);

    if (m_hasTransform = entity->HasComponent<engine::Transform>())
        m_transform->SetTransform(m_graph->GetComponent<engine::Transform>(m_handle));
}
