#include "ui/Properties.h"
#include "ui/components/CameraComponent.h"
#include "ui/components/RendererComponent.h"
#include "ui/components/ScriptComponent.h"
#include "ui/components/TransformComponent.h"
#include "ui/components/RigidBodyStaticComponent.h"
#include "ui/components/RigidBodyDynamicComponent.h"

#include <engine/ui/UIComponent.h>
#include <engine/core/SceneGraph.h>
#include <engine/utility/MemoryCheck.h>

#define WINDOW_NAME "Properties"
#define INVALID_HANDLE engine::Entity::EHandleUtils::INVALID_HANDLE

editor::PropertyWnd::PropertyWnd(engine::SceneGraph* graph)
{
    SetName(WINDOW_NAME);
    SetFlags(::ui::EWndFlags::MENU_BAR);

    m_handle = INVALID_HANDLE;
    m_graph = graph;
}

editor::PropertyWnd::~PropertyWnd(void)
{
    ClearComponentArray();

    m_handle = INVALID_HANDLE;
    m_graph = nullptr;
}

void editor::PropertyWnd::SetHandle(engine::EntityHandle handle)
{
    m_handle = handle;

    InitComponents();
}

engine::EntityHandle editor::PropertyWnd::GetHandle(void) const noexcept
{
    return m_handle;
}

void editor::PropertyWnd::RenderContents(void)
{
    RenderMenuBar();

    if (m_handle == INVALID_HANDLE)
        return;

    // Render all components
    for (BaseComponent* component : m_components)
    {
        component->RenderSection(m_graph, m_handle);

        if (component->IsRemoved())
        {
            InitComponents();
            break;
        }

    }

}

void editor::PropertyWnd::InitComponents(void)
{
    ClearComponentArray();
    
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

    // RigidBodyStatic
    if (entity->HasComponent<engine::RigidBodyStatic>())
        InitComponent<RigidBodyStaticComponent, engine::RigidBodyStatic>();

    // RigidBodyDynamic
    if (entity->HasComponent<engine::RigidBodyDynamic>())
        InitComponent<RigidBodyDynamicComponent, engine::RigidBodyDynamic>();
    
    // Script
    if (entity->HasComponent<engine::Script>())
    {
        engine::Script* scriptComponent = m_graph->GetComponent<engine::Script>(m_handle);

        for (engine::ScriptObject script : scriptComponent->GetScripts())
            m_components.emplace_back(new ScriptComponent(script.GetType()));
    }
}

void editor::PropertyWnd::RenderMenuBar(void)
{
    ui::StartDisabledSection(!m_graph || m_handle == INVALID_HANDLE);
    if (ui::StartMenuBar())
    {
        if (ui::StartMenu("Add Component"))
        {
                 if (ui::MenuItem("Renderer"))
                AddComponent<RendererComponent, engine::Renderer>();

            else if (ui::MenuItem("RigidbodyStatic"))
                AddComponent<RigidBodyStaticComponent, engine::RigidBodyStatic>();

            else if (ui::MenuItem("RigidbodyDynamic"))
                AddComponent<RigidBodyDynamicComponent, engine::RigidBodyDynamic>();
            
            else if (ui::MenuItem("Camera"))
                AddComponent<CameraComponent, engine::Camera>();
            
             else if (ui::MenuItem("Script"))
                printf("Adding script...\n"); // TODO: implement for script object
            
             else if (ui::MenuItem("Transform"))
                AddComponent<TransformComponent, engine::Transform>();

            ui::EndMenu();
        }
        
        ui::EndMenuBar();
    }
    ui::EndDisabledSection();
}

void editor::PropertyWnd::ClearComponentArray(void)
{
    for (BaseComponent* component : m_components)
    {
        // Call the derrived class destructor
        switch (component->GetType())
        {
        case editor::AUDIO:
            break;
        case editor::CAMERA:
            delete dynamic_cast<CameraComponent*>(component);
            break;
        case editor::RENDERER:
            delete dynamic_cast<RendererComponent*>(component);
            break;
        case editor::RIGIDBODY_STATIC:
            delete dynamic_cast<RigidBodyStaticComponent*>(component);
            break;
        case editor::RIGIDBODY_DYNAMIC:
            delete dynamic_cast<RigidBodyDynamicComponent*>(component);
            break;
        case editor::SCRIPT:
            delete dynamic_cast<ScriptComponent*>(component);
            break;
        case editor::TRANSFORM:
            delete dynamic_cast<TransformComponent*>(component);
            break;
        case editor::INVALID_COMPONENT_TYPE:
        default:
            delete component;
            break;
        }
    }

    m_components.clear();
}
