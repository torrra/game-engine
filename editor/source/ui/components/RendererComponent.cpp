#include "ui/components/RendererComponent.h"
#include "ui/Assets.h"

#include <engine/resource/model/Model.h>
#include <engine/resource/shader/Shader.h>
#include <engine/resource/ResourceManager.h>
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDragDrop.h>

editor::RendererComponent::RendererComponent(void)
{
    SetName("Renderer");
    SetType(RENDERER);
}

editor::RendererComponent::~RendererComponent(void)
{
    SetData<engine::Renderer>(nullptr);
}

void editor::RendererComponent::SectionContent(void)
{
    // Get renderer data
    engine::Renderer* rendererData = GetData<engine::Renderer>();

    ModelInput(rendererData);
    ShaderInput(rendererData);
}

void editor::RendererComponent::ModelInput(engine::Renderer* renderer)
{
    // Get file name
    std::string modelName("None");

    if (const engine::Model* model = renderer->GetModel())
    {
        std::string path = model->GetName();
        modelName = path;

        uint64 offset = path.rfind('\\') + 1;
        if (offset != path.npos);
        modelName = modelName.substr(offset);
    }

    // Model input
    ui::Text("Model: ");
    ui::SameLine(110.0f);
    ui::Button(modelName.c_str());

    // Drag / drop
    if (ui::StartDragDropTarget())
    {
        // Check asset type
        if (const ui::Payload payload = ui::AcceptPayload(MODEL_PAYLOAD, 0))
        {
            Asset* payloadData = reinterpret_cast<Asset*>(payload.GetData());
            engine::ResourceManager::Load<engine::Model>(payloadData->m_path.string());
            renderer->SetModel(payloadData->m_path.string().c_str());
        }

        ui::EndDragDropTarget();
    }

    // Formatting
    ui::VerticalSpacing();
}

void editor::RendererComponent::ShaderInput(engine::Renderer* renderer)
{
    // Get shader program name
    std::string shaderName("None");

    if (const engine::ShaderProgram* shader = renderer->GetShader())
        shaderName = shader->GetName();
    else
        renderer->SetShader("ModelTextured");

    ui::Text("Shader: ");
    ui::SameLine(110.0f);
    ui::Button(shaderName.c_str());
    ui::VerticalSpacing();

}
