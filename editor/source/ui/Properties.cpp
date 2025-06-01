#include "ui/Properties.h"
#include "ui/Assets.h"
#include "ui/components/CameraComponent.h"
#include "ui/components/RendererComponent.h"
#include "ui/components/ScriptComponent.h"
#include "ui/components/TransformComponent.h"
#include "ui/components/LightSourceComponent.h"
#include "ui/components/RigidBodyStaticComponent.h"
#include "ui/components/RigidBodyDynamicComponent.h"
#include "ui/components/NavigationPointComponent.h"
#include "ui/components/AudioComponent.h"

#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDragDrop.h>
#include <engine/ui/UIDraw.h>
#include <engine/ui/UIStyle.h>
#include <engine/ui/InternalUIWindow.h>

#include <engine/core/SceneGraph.h>
#include <engine/core/components/LightSource.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/ConsoleLog.hpp>

#define WINDOW_NAME "Properties"
#define INVALID_HANDLE engine::Entity::EHandleUtils::INVALID_HANDLE

editor::PropertyWnd::PropertyWnd(engine::SceneGraph* graph)
{
    SetName(WINDOW_NAME);
    SetFlags(::ui::EWndFlags::MENU_BAR);

    m_handle = INVALID_HANDLE;
    m_graph = graph;
}

editor::PropertyWnd::PropertyWnd(PropertyWnd&& other) noexcept
{
    m_flags = other.m_flags;
    m_graph = other.m_graph;
    m_handle = other.m_handle;
    m_title = std::move(other.m_title);
    m_components = std::move(other.m_components);

    other.m_flags = 0;
    other.m_graph = nullptr;
    other.m_handle = -1;
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

    ScriptInput();
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

    if (entity->HasComponent<engine::LightSource>())
        InitComponent<LightSourceComponent, engine::LightSource>();
    

    // AudioPlayer
    if (entity->HasComponent<engine::AudioPlayer>())
        InitComponent<AudioComponent, engine::AudioPlayer>();
    
    // Script
    if (entity->HasComponent<engine::Script>())
    {
        engine::Script* scriptComponent = m_graph->GetComponent<engine::Script>(m_handle);

        for (const engine::ScriptObject& script : scriptComponent->GetScripts())
        {
            if (!script.IsValid())
                continue;

            ScriptComponent* newScript = new ScriptComponent(script.GetType());
            newScript->SetScript(script.GetIndex());
            
            m_components.emplace_back(newScript);
        }
    }

    // Navigation point
    if (entity->HasComponent<engine::NavigationPoint>())
        InitComponent<NavigationPointComponent, engine::NavigationPoint>();
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
            {
                AddComponent<RigidBodyStaticComponent, engine::RigidBodyStatic>();
                if (engine::RigidBodyStatic* rbStatic = m_graph->GetComponent<engine::RigidBodyStatic>(m_handle))
                    rbStatic->SwitchShape(rbStatic->GetShape());
            }

            else if (ui::MenuItem("RigidbodyDynamic"))
            {
                AddComponent<RigidBodyDynamicComponent, engine::RigidBodyDynamic>();
                if (engine::RigidBodyDynamic* rbDynamic = m_graph->GetComponent<engine::RigidBodyDynamic>(m_handle))
                    rbDynamic->SwitchShape(rbDynamic->GetShape());
            }

            else if (ui::MenuItem("Camera"))
                AddComponent<CameraComponent, engine::Camera>();

            else if (ui::MenuItem("Transform"))
                AddComponent<TransformComponent, engine::Transform>();

            else if (ui::MenuItem("Light source"))
                AddComponent<LightSourceComponent, engine::LightSource>();

            else if (ui::MenuItem("NavigationPoint"))
                AddComponent<NavigationPointComponent, engine::NavigationPoint>();

            else if (ui::MenuItem("AudioPlayer"))
                AddComponent<AudioComponent, engine::AudioPlayer>();

            ui::EndMenu();
        }
        
        ui::EndMenuBar();
    }
    ui::EndDisabledSection();
}

void editor::PropertyWnd::ScriptInput(void)
{
    if (::ui::StartDragDropTargetCustom(GetWindowRect(), m_title.c_str()))
    {
        // Check asset type
        if (const ui::Payload payload = ui::AcceptPayload(SCRIPT_PAYLOAD, 0))
        {
            if (payload.IsPreview())
            {
                ::ui::DrawList drawList = ::ui::GetForegroundDrawList();
                auto transform = this->GetWindowRect();
                uint32 color = ::ui::GetColor(::ui::DRAG_DROP_TARGET_COLOR, 0.05f);
                drawList.AddRectFilled(transform.m_min, transform.m_max, color);
            }

            Asset* payloadData = reinterpret_cast<Asset*>(payload.GetData());

            if (payload.IsDelivery())
            {
                std::filesystem::path filename = payloadData->m_path.filename().replace_extension();

                if (engine::Script* script = m_graph->GetComponent<engine::Script>(m_handle))
                    AddScript(script, filename.string());

                else if (engine::Script* newScript = m_graph->CreateComponent<engine::Script>(m_handle))
                    AddScript(newScript, filename.string());

                else
                    engine::PrintLog(engine::ErrorPreset(), "Unable to add script to entity");

                InitComponents();
            }
        }
        ui::EndDragDropTarget();
    }
    // Formatting
    ui::VerticalSpacing();
}

void editor::PropertyWnd::AddScript(engine::Script* script, const std::string& name)
{
    for (const engine::ScriptObject& object : script->GetScripts())
    {
        if (object.IsValid() && name == object.GetName())
            return;
    }

    script->AddScriptObject(name);
}

void editor::PropertyWnd::ClearComponentArray(void)
{
    for (BaseComponent* component : m_components)
    {
        // Call the derrived class destructor
        switch (component->GetType())
        {
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
        case editor::LIGHT_SOURCE:
            delete dynamic_cast<LightSourceComponent*>(component);
            break;
        case editor::NAVIGATION_POINT:
            delete dynamic_cast<NavigationPointComponent*>(component);
            break;
        case editor::AUDIO:
            delete dynamic_cast<AudioComponent*>(component);
            break;
        case editor::INVALID_COMPONENT_TYPE:
        default:
            delete component;
            break;
        }
    }

    m_components.clear();
}

editor::PropertyWnd& editor::PropertyWnd::operator=(PropertyWnd&& rhs) noexcept
{
    m_flags = rhs.m_flags;
    m_graph = rhs.m_graph;
    m_handle = rhs.m_handle;
    m_title = std::move(rhs.m_title);
    m_components = std::move(rhs.m_components);

    rhs.m_flags = 0;
    rhs.m_graph = nullptr;
    rhs.m_handle = -1;
    return *this;
}
