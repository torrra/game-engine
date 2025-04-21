#include <math/Matrix4.hpp>

#include "core/components/Renderer.h"
#include "core/components/Transform.h"

#include "resource/model/Model.h"
#include "resource/shader/Shader.h"
#include "resource/texture/Texture.h"
#include "resource/ResourceManager.h"

#include "core/SceneGraph.h"

#include "serialization/TextSerializer.h"

#include "glad/glad.h"

namespace engine
{
    void Renderer::Register(void)
    {
    }

    void Renderer::Render(const math::Matrix4f& viewProjection,
                          ComponentArray<class Transform>& transforms)
    {
        if (!m_model || !m_shader)
            return;

        if (!m_model->CanRender())
            return;

        m_shader->Use();

        if (Transform* transform = transforms.GetComponent(m_owner))
        {
            math::Matrix4f transformMat = Transform::ToWorldMatrix(*transform);
            math::Matrix4f mvp = viewProjection * transformMat;

            math::Matrix4f normalMat4x4 = transformMat.Inverse().Transpose();
            math::Matrix3f normalMat3x3 = math::Matrix3f(normalMat4x4);

            m_shader->Set("model", &transformMat);
            m_shader->Set("mvp", &mvp);
            m_shader->Set("normalMat", &normalMat3x3);		
        }
        else
        {
            math::Matrix3f identityMat{ 1.f };

            m_shader->Set("mvp", &viewProjection);
            m_shader->Set("normalMat", &identityMat);
        }

        if (m_materials.empty())
            m_materials.resize(m_model->GetMeshCount());

        m_model->Draw(m_materials);
    }

    const Model* Renderer::GetModel(void) const
    {
        return m_model;
    }

    const ShaderProgram* Renderer::GetShader(void) const
    {
        return m_shader;
    }

    const MeshMaterial* Renderer::GetMaterial(uint32 index) const
    {
        if (index < m_materials.size())
            return m_materials.at(index);

        else
            return nullptr;
    }

    void Renderer::SetMaterial(uint32 index, const MeshMaterial* material)
    {
        if (index >= m_materials.size())
            m_materials.resize(static_cast<uint64>(index + 1));

        m_materials[index] = material;
    }

    void Renderer::SetMaterial(uint32 index, const char* key)
    {
        if (const MeshMaterial* mat = ResourceManager::GetResource<MeshMaterial>(key))
            SetMaterial(index, mat);
    }

    void Renderer::SetModel(const Model* model)
    {
        m_model = model;
        m_materials.resize(model->GetMeshCount()); 
    }

    void Renderer::SetModel(const char* key)
    {
        if (const Model* model = ResourceManager::GetResource<Model>(key))
            SetModel(model);
    }

    void Renderer::SetShader(const ShaderProgram* shader)
    {
        m_shader = shader;
    }

    void Renderer::SetShader(const char* key)
    {
        if (const ShaderProgram* shader = ResourceManager::GetResource<ShaderProgram>(key))
            m_shader = shader;
    }

    void Renderer::SerializeText(std::ostream& output, EntityHandle owner,
                                 uint64 index) const
    {

        const std::string* model = ResourceManager::FindKeyByVal(m_model);
        const std::string* shader = ResourceManager::FindKeyByVal(m_shader);

        if ((!model) || (!shader))
            return;

        output << "[Renderer]\n    ";

        if constexpr (UpdateAfterParent<Renderer>::m_value)
        {
            text::Serialize(output, "index", index);
            output << "\n    ";
        }

        text::Serialize(output, "owner", owner);

        output << "\n    ";
        text::Serialize(output, "model", *model);
        output << "\n    ";
        text::Serialize(output, "shader", *shader);
        output << "\n    ";

        text::Serialize(output, "flags", m_flags);
        output << '\n';
    }

    const char* Renderer::DeserializeText(const char* text, const char* end)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, m_owner);

        std::string key;
        text = text::DeserializeString(text, end, key);

        m_model = ResourceManager::GetResource<Model>(key);

        key.clear();
        text = text::DeserializeString(text, end, key);
        m_shader = ResourceManager::GetResource<ShaderProgram>(key);

        MOVE_TEXT_CURSOR(text, end);
        return text::DeserializeInteger(text, m_flags);
    }

}