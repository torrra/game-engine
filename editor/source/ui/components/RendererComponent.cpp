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

    m_modelName = "None##model";
    m_fragName = "None##shader";
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
    std::string modelName("None##model");

    //if (const engine::Model* model = renderer->GetModel())
    //{
    //    std::string path = model->GetName();
    //    modelName = path;
    //
    //    uint64 offset = path.rfind('\\') + 1;
    //    if (offset != path.npos);
    //    modelName = modelName.substr(offset);
    //}

    // Model input
    ui::Text("Model: ");
    ui::SameLine(150.0f);
    ui::Button(m_modelName.c_str());

    // Drag / drop
    if (ui::StartDragDropTarget())
    {
        // Check asset type
        if (const ui::Payload payload = ui::AcceptPayload(MODEL_PAYLOAD, 0))
        {
            Asset* payloadData = reinterpret_cast<Asset*>(payload.GetData());
            engine::ResourceManager::Load<engine::Model>(payloadData->m_path.string());
            m_modelName = payloadData->m_path.string().c_str();
            renderer->SetModel(payloadData->m_path.string().c_str());
        }

        ui::EndDragDropTarget();
    }

    // Formatting
    ui::VerticalSpacing();
}

void editor::RendererComponent::ShaderInput(engine::Renderer* renderer)
{
    ui::Text("Fragment Shader: ");
    ui::SameLine(150.0f);
    ui::Button(m_fragName.c_str());

    // Drag / drop
    if (ui::StartDragDropTarget())
    {
        // Check asset type
        if (const ui::Payload payload = ui::AcceptPayload(FRAGMENT_SHADER_PAYLOAD, 0))
        {
            constexpr const char* defaultVertexShader = ".\\shaders\\ModelTextured.vert";

            Asset* payloadData = reinterpret_cast<Asset*>(payload.GetData());
            
            // Create shader name
            uint64 startOffset = payloadData->m_fileName.rfind('\\') + 1;
            uint64 endOffset = payloadData->m_fileName.rfind('.');

            if (startOffset < endOffset)
            {
                m_fragName = payloadData->m_fileName.substr(startOffset);
                std::string programName = "program" + m_fragName.substr(0, endOffset);

                engine::ResourceManager::LoadShader(programName.c_str(), defaultVertexShader, payloadData->m_path.string().c_str());
                renderer->SetShader(programName.c_str());
            }
        }

        ui::EndDragDropTarget();
    }

    ui::VerticalSpacing();

}
