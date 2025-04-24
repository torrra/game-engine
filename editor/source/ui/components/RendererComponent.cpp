#include "ui/components/RendererComponent.h"
#include "ui/Assets.h"
#include <engine/core/components/Renderer.h>
#include <engine/resource/model/Model.h>
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDragDrop.h>
#include <engine/resource/ResourceManager.h>

editor::RendererComponent::RendererComponent(void)
{
    SetName("Renderer");
}

editor::RendererComponent::~RendererComponent(void)
{
    SetData<engine::Renderer>(nullptr);
}

void editor::RendererComponent::SectionContent(void)
{
    engine::Renderer* rendererData = GetData<engine::Renderer>();
    std::string modelName;

    if (const engine::Model* model = rendererData->GetModel())
    {
        std::string path = model->GetName();
        modelName = path;

        uint64 offset = path.rfind('\\') + 1;
        if (offset != path.npos);
            modelName = modelName.substr(offset);
    }
    
    ui::Text("Model: to do"); // TODO: add input for model (need asset window)
    ui::Button(modelName.c_str());
    
    if (ui::StartDragDropTarget())
    {
        if (const ui::Payload payload = ui::AcceptPayload("Asset", 0))
        {
            Asset* payloadData = reinterpret_cast<Asset*>(payload.GetData());
            
            uint64 offset = payloadData->m_fileName.rfind('.');

            if (offset != payloadData->m_fileName.npos && payloadData->m_fileName[offset + 1] != '\0')
            {
                std::string extension = payloadData->m_fileName.substr(offset);

                static const std::string validExtensions[] = {".obj", ".fbx", ".dae"};
                
                bool isModel = false;
                for (const std::string& validExtension : validExtensions)
                {
                    isModel = (extension == validExtension);

                    if (isModel)
                        break;
                }

                if (isModel)
                {
                    engine::ResourceManager::Load<engine::Model>(payloadData->m_path.string());
                    rendererData->SetModel(payloadData->m_path.string().c_str());

                }
            }
        }

        ui::EndDragDropTarget();
    }

    ui::Text("Shader: to do"); // TODO: add input for shader (need asset window)
}
