#include "ui/components/RendererComponent.h"
#include "ui/Assets.h"

#include <engine/resource/model/Model.h>
#include <engine/resource/shader/Shader.h>
#include <engine/resource/material/MeshMaterial.h>
#include <engine/resource/ResourceManager.h>
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDragDrop.h>
#include <engine/ui/UIStyle.h>

#define NO_MODEL_NAME "None##model"
#define NO_FRAG_NAME "None##shader"

editor::RendererComponent::RendererComponent(void)
{
    SetName("Renderer");
    SetType(RENDERER);

    m_modelName = NO_MODEL_NAME;
    m_fragName = NO_FRAG_NAME;
}

editor::RendererComponent::~RendererComponent(void)
{
}

void editor::RendererComponent::SectionContent(void)
{
    // Get renderer data
    engine::Renderer* rendererData = GetData<engine::Renderer>();

    if (rendererData)
    {
        ModelInput(rendererData);
        ShaderInput(rendererData);
        RenderMaterialList(rendererData);

        if (ui::Button("Add material", { 100.f, 25.f }))
        {
            rendererData->SetMaterial(rendererData->GetMaterialCount(),
                                      (const engine::MeshMaterial*)nullptr);
        }
    }
}

void editor::RendererComponent::ModelInput(engine::Renderer* renderer)
{
    // Get file name
    m_modelName = (renderer->GetModel()) ? renderer->GetModel()->GetName() : NO_MODEL_NAME;
    m_fragName = (renderer->GetShader()) ? renderer->GetShader()->GetFragmentShaderName() : NO_FRAG_NAME;

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
            m_modelName = payloadData->m_path.filename().string().c_str();
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

                engine::ResourceManager::LoadShader(programName.c_str(),
                                                    defaultVertexShader,
                                                    payloadData->m_path.string().c_str(), true);

                renderer->SetShader(programName.c_str());
            }
        }

        ui::EndDragDropTarget();
    }

    ui::VerticalSpacing();

}

void editor::RendererComponent::RenderMaterialList(engine::Renderer* renderer)
{
    const uint32 count = renderer->GetMaterialCount();
    const std::string matPrefix = "Material ";

    if (!count)
        return;

    std::string name;

    if (ui::TreeNode("Materials"))
    {
        ui::StartSection("##materials");
        for (uint32 matNum = 0; matNum < count; ++matNum)
        {
            name = matPrefix + std::to_string(matNum);
            MaterialInput(renderer, name, matNum);
        }

        ui::EndSection();
        ui::TreePop();
    }

    ui::VerticalSpacing();
}

void editor::RendererComponent::MaterialInput(engine::Renderer* renderer, const std::string& name,
                                               uint32 index)
{
    ui::Text(name.c_str());
    ui::SameLine(150.f);

    std::string matName;

    const engine::MeshMaterial* material = renderer->GetMaterial(index);

    if (material)
    {
        matName = material->GetFilePath().filename().string();
        matName.append(" (") += std::to_string(index);
        matName.push_back(')');
    }
     else
       matName = name;
    
    if (ui::Button(matName.c_str()))
        renderer->SetMaterial(index, (const engine::MeshMaterial*)nullptr);


    // Drag / drop
    if (ui::StartDragDropTarget())
    {
        // Check asset type
        if (const ui::Payload payload = ui::AcceptPayload(MATERIAL_PAYLOAD, 0))
        {
            Asset* payloadData = reinterpret_cast<Asset*>(payload.GetData());
            std::string filePath = payloadData->m_path.string();

            engine::ResourceManager::Load<engine::MeshMaterial>(filePath);
            renderer->SetMaterial(index, filePath.c_str());      
        }

        ui::EndDragDropTarget();
    }

    ui::VerticalSpacing();

}
