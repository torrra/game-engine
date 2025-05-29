#include "ui/AssetDetails.h"
#include "ui/Assets.h"

#include <engine/resource/ResourceManager.h>
#include <engine/resource/material/MeshMaterial.h>
#include <engine/resource/texture/Texture.h>
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIDragDrop.h>

// leave data union uninitialized as it will be initialized and
// destroyed in SelectAsset depending on asset type
// Initializing it from the start could
// cause STL container proxies to never be deallocated until after
// leak check
#pragma warning(push)
#pragma warning(disable: 26495)
editor::AssetDetailsWnd::AssetDetailsWnd(void)
{
    SetName("Asset Details");
}
#pragma warning(pop)

editor::AssetDetailsWnd::~AssetDetailsWnd(void)
{
    ResetData();
}

void editor::AssetDetailsWnd::SelectAsset(const std::string& path, EAssetType type)
{
    ResetData();
    engine::EditableRef<engine::MeshMaterial> mat;

    switch (type)
    {
    case EAssetType::MATERIAL:
        engine::ResourceManager::Load<engine::MeshMaterial>(path);


        m_selectedResource =
        engine::ResourceManager::GetEditableResource<engine::MeshMaterial>(path);

        InitMaterialData();      
        break;

    default:
        break;
    }

    m_selectedAssetType = type;
}

void editor::AssetDetailsWnd::RenderContents(void)
{
    switch (m_selectedAssetType)
    {
    case editor::AssetDetailsWnd::EAssetType::MATERIAL:
        RenderMaterialWindow();
        break;
    default:
        break;
    }
}

void editor::AssetDetailsWnd::RenderMaterialWindow(void)
{
    engine::MeshMaterial* material = dynamic_cast<engine::MeshMaterial*>(&*m_selectedResource);

    if (!material)
        return;

    ui::Separator("Texture maps");
    RenderMaterialTextureInput(material);

    ui::Separator("Color");
    RenderMaterialColorInput(material);
    ui::VerticalSpacing();
    
    RenderMaterialInputBoxes(material);
    ui::VerticalSpacing();
    RenderMaterialSaveButton(material);
}

void editor::AssetDetailsWnd::RenderMaterialTextureInput(engine::MeshMaterial* material)
{
    constexpr uint64 mapNameCount = sizeof(m_matData.m_mapNames) / sizeof(m_matData.m_mapNames[0]);

    for (uint64 index = 0; index < mapNameCount; ++index)
    {
        // Model input
        ui::Text(GetTextureName(index).c_str());
        ui::SameLine(150.0f);
        
        if (ui::Button(m_matData.m_mapNames[index].c_str()))
        {
            material->SetTexture(engine::ResourceRef<engine::Texture>(), index);
            m_matData.m_mapNames[index] = "No " + GetTextureName(index);
        }

        // Drag / drop
        if (ui::StartDragDropTarget())
        {
            // Check asset type
            if (const ui::Payload payload = ui::AcceptPayload(TEXTURE_PAYLOAD, 0))
            {
                Asset* payloadData = reinterpret_cast<Asset*>(payload.GetData());
                std::string pathStr = payloadData->m_path.string();
                engine::ResourceManager::Load<engine::Texture>(pathStr);

                m_matData.m_mapNames[index] = pathStr;
                material->SetTexture(engine::ResourceManager::GetResource<engine::Texture>(pathStr), index);
            }

            ui::EndDragDropTarget();
        }

        // Formatting
        ui::VerticalSpacing();
    }
}

void editor::AssetDetailsWnd::RenderMaterialColorInput(engine::MeshMaterial* material)
{
    math::Vector3f ambient = material->GetAmbient();
    math::Vector3f diffuse = material->GetDiffuse();
    math::Vector3f specular = material->GetSpecular();
    math::Vector3f emissive = material->GetEmissive();

    ui::Text("Ambient: ");
    ui::SameLine(80.f);
    
    if(ui::ColorEdit("##ambient mat", (f32*) &ambient))
        material->SetAmbient(ambient);

    ui::VerticalSpacing();

    ui::Text("Diffuse: ");
    ui::SameLine(80.f);
    
    if(ui::ColorEdit("##diffuse mat", (f32*)&diffuse))
        material->SetDiffuse(diffuse);

    ui::VerticalSpacing();

    ui::Text("Specular: ");
    ui::SameLine(80.f);

    if (ui::ColorEdit("##specular mat", (f32*)&specular))
        material->SetSpecular(specular);

    ui::VerticalSpacing();
    
    ui::Text("Emissive: ");
    ui::SameLine(80.f);

    if (ui::ColorEdit("##emissive mat", (f32*)&emissive))
        material->SetEmissive(emissive);
}

void editor::AssetDetailsWnd::RenderMaterialInputBoxes(engine::MeshMaterial* material)
{
    ui::Separator();
    ui::VerticalSpacing();

    f32 shininess = material->GetShininess();

    ui::Text("Shininess: ");
    ui::SameLine(150.f);
    ui::ItemWidth(80.f);

    if (ui::InputBox("##shininess", &shininess, 0.5f))
        material->SetShininess(math::Max(0.f, shininess));

    ui::VerticalSpacing();

    f32 refractionIndex = material->GetRefractionIndex();

    ui::Text("Refraction index: ");
    ui::SameLine(150.f);
    ui::ItemWidth(80.f);

    if (ui::InputBox("##refraction index", &refractionIndex, 0.5f))
        material->SetRefractionIndex(refractionIndex);

    ui::VerticalSpacing();

    f32 opacity = material->GetOpacity();

    ui::Text("Opacity: ");
    ui::SameLine(150.f);
    ui::ItemWidth(80.f);

    if (ui::InputBox("##opacity", &opacity, 0.01f))
        material->SetOpacity(math::Clamp(opacity, 0.f, 1.f));

    ui::VerticalSpacing();
}

void editor::AssetDetailsWnd::RenderMaterialSaveButton(engine::MeshMaterial* material)
{
    if (ui::Button("Save asset", { 100.f, 25.f }))
        material->SerializeText();

    ui::VerticalSpacing();
}

std::string editor::AssetDetailsWnd::GetTextureName(uint64 index)
{
    engine::MeshMaterial::EMapIndex mapIndex = static_cast<engine::MeshMaterial::EMapIndex>(index);

    switch (mapIndex)
    {
    case engine::MeshMaterial::DIFFUSE:
        return "Diffuse map ";
    case engine::MeshMaterial::NORMAL:
        return "Normal map ";
    case engine::MeshMaterial::SPECULAR:
        return "Specular map ";
    case engine::MeshMaterial::ROUGHNESS:
        return "Roughness map ";
    case engine::MeshMaterial::AMBIENT_OCCLUSION:
        return "AO map ";
    default:
        return "Invalid map";
    }
}

void editor::AssetDetailsWnd::ResetData(void)
{
    switch (m_selectedAssetType)
    {
    case EAssetType::MATERIAL:
        DestroyMaterialData();
        break;

    default:
        break;
    }
}

void editor::AssetDetailsWnd::DestroyMaterialData(void)
{
    for (std::string& string : m_matData.m_mapNames)
        string.~basic_string();
}

// undef new macro to use placement new
#undef new
void editor::AssetDetailsWnd::InitMaterialData(void)
{
    constexpr uint64 mapNameCount = sizeof(m_matData.m_mapNames) / sizeof(m_matData.m_mapNames[0]);

    for (uint64 index = 0; index < mapNameCount; ++index)
        new(m_matData.m_mapNames + index) std::string();

    engine::MeshMaterial* material = dynamic_cast<engine::MeshMaterial*>(&*m_selectedResource);

    if (!material)
        return;

    const engine::ResourceRef<engine::Texture>* textures = material->GetTextures();

    for (uint64 index = 0; index < mapNameCount; ++index)
    {
        const std::string* textureName = engine::ResourceManager::FindKeyByVal<engine::Texture>(textures[index]);
        m_matData.m_mapNames[index] = (textureName) ? *textureName : "No " + GetTextureName(index);
    }

}
