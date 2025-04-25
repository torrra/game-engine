#include "Picking.h"

#include <engine/utility/GraphicsHelperFunctions.h>
#include <engine/ui/UIComponent.h>
#include <engine/resource/ResourceManager.h>
#include <engine/core/Entity.h>
#include <engine/resource/model/Model.h>
#include <engine/input/Input.h>

#define INVALID_HANDLE engine::Entity::EHandleUtils::INVALID_HANDLE
#define SHADER_NAME "Picking Shader"

editor::PickableEntity::PickableEntity(int64 const& handle)
    : m_entityHandle(handle)
{
    static uint32 uniqueID = 0;
    m_pickingID = ++uniqueID;
   
    // Calculate the color via the entities picking identifier
    uint8 r = (m_pickingID & 0x000000FF) >>  0;
    uint8 g = (m_pickingID & 0x0000FF00) >>  8;
    uint8 b = (m_pickingID & 0x00FF0000) >> 16;

    // Convert color components (red, green, blue) to float within 0 - 1 range
    constexpr f32 denominator = 1.0f / 255.0f;
    m_color =
    {
        static_cast<f32>(r) * denominator,
        static_cast<f32>(g) * denominator,
        static_cast<f32>(b) * denominator
    };
}

uint32 editor::PickableEntity::GetPickingID(void) const
{
    return m_pickingID;
}

int64 editor::PickableEntity::GetEntityHandle(void) const
{
    return m_entityHandle;
}

math::Vector3f editor::PickableEntity::GetColor(void) const
{
    return m_color;
}

editor::Picking::Picking(engine::SceneGraph* graph)
{
    // Load shader - this shader sends the unique color for each entity to the GPU
    engine::ResourceManager::LoadShader(
        SHADER_NAME,
        ".\\shaders\\Picking.vert",
        ".\\shaders\\Picking.frag"
    );
    
    // Get shader program
    m_pickingShader = engine::ResourceManager::GetResource<engine::ShaderProgram>(SHADER_NAME);

    // Create structure for each entity in scene
    for (engine::EntityHandle handle : graph->GetChildrenAllLevels(INVALID_HANDLE))
    {
        PickableEntity entity(handle);
        m_pickableEntity[entity.GetPickingID()] = entity;
    }
}

editor::Picking::~Picking(void)
{
    m_pickingShader = nullptr;
}

void editor::Picking::RenderSceneColored(engine::SceneGraph* graph)
{
    for (auto pickableEntity : m_pickableEntity)
    {
        engine::EntityHandle handle = pickableEntity.second.GetEntityHandle();
        engine::Renderer* renderer = graph->GetComponent<engine::Renderer>(handle);

        // To select an entity it must have a model & therefore a renderer component
        if (!renderer)
            continue;
        
        const engine::Model* model = renderer->GetModel();
        if (!model)
            continue;
        
        // Set uniforms
        math::Matrix4f mvp = renderer->GetMVP();
        m_pickingShader->Use();
        m_pickingShader->Set("mvp", &mvp);
        m_pickingShader->Set("pickingColor", math::Vector3f(pickableEntity.second.GetColor()));

        // Render picking stage (render models as block colors based on their identifier)
        std::vector<const engine::MeshMaterial*> materialArray(model->GetMeshCount());
        model->Draw(materialArray);
    }
}

engine::EntityHandle editor::Picking::FindSelectedEntity(void) const
{
    engine::SendCmdsToGPU();
    engine::SetPixelStorageMode();

    // Get window position
    math::Vector2i windowPos = ui::GetViewportPos("Viewport");
    
    // Get window padding
    const math::Vector2i& windowPadding = 
        ui::GetInnerRectMinPos("Viewport") - ui::GetOuterRectMinPos("Viewport");
    
    // Calculate mouse position
    math::Vector2i mousePos = engine::Input::GetCursorPosition<int32>() - windowPadding;
    
    // Adjust mouse position if window is docked
    if (ui::IsWindowDocked("Viewport"))
    {
        windowPos = ui::GetWindowPos("Viewport") - windowPos;
        mousePos -= windowPos;
    }

    // Get pixel color data at mouse position
    uint8 data[4];
    engine::GetPixelData(mousePos, {1, 1}, data);

    // Convert color back into picking ID
    uint32 pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256;

    // Attempt to find the related entity via its picking identifier
    auto entity = m_pickableEntity.find(pickedID);
    return (entity != m_pickableEntity.end()) ? entity->second.GetEntityHandle() : INVALID_HANDLE;
}
