#include <math/Matrix4.hpp>

#include "core/components/Renderer.h"
#include "core/components/Transform.h"

#include "resource/model/Model.h"
#include "resource/shader/Shader.h"
#include "resource/texture/Texture.h"
#include "resource/ResourceManager.h"
#include "utility/ResourceRef.h"

#include "core/SceneGraph.h"

#include "serialization/TextSerializer.h"

#include <glad/glad.h>

#include "InternalOpenGLError.hpp"

namespace engine
{
    Renderer::Renderer(EntityHandle owner, SceneGraph* scene)
        : Component(owner, scene)
    {

    }

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
            math::Matrix4f identity4x4{ 1.f };
            math::Matrix3f identity3x3{ 1.f };

            m_shader->Set("model", &identity4x4);
            m_shader->Set("mvp", &viewProjection);
            m_shader->Set("normalMat", &identity3x3);
        }

        if (m_model->IsDynamic() && (m_animator.IsPlaying() || m_animator.IsPaused()))
        {
            m_animator.UseSkinningBuffer();
            m_shader->Set("isRigged", true);
        }
        else
            m_shader->Set("isRigged", false);

        m_model->Draw(m_materials);
        OpenGLError();
    }

    const ResourceRef<Model>& Renderer::GetModel(void) const
    {
        return m_model;
    }

    const ResourceRef<ShaderProgram>& Renderer::GetShader(void) const
    {
        return m_shader;
    }

    const ResourceRef<MeshMaterial>& Renderer::GetMaterial(uint32 index) const
    {
        return m_materials.at(index);
    }

    uint32 Renderer::GetMaterialCount(void) const
    {
        return static_cast<uint32>(m_materials.size());
    }


    void Renderer::SetMaterial(uint32 index, ResourceRef<MeshMaterial>&& material)
    {
        if (index >= m_materials.size())
            m_materials.resize(static_cast<uint64>(index + 1));

        m_materials[index] = std::forward<ResourceRef<MeshMaterial>>(material);
    }

    void Renderer::SetMaterial(uint32 index, const char* key)
    {
        ResourceRef<MeshMaterial> mat = ResourceManager::GetResource<MeshMaterial>(key);
        SetMaterial(index, std::move(mat));
    }

    void Renderer::SetModel(ResourceRef<Model>&& model)
    {
        m_model = std::forward<ResourceRef<Model>>(model);
        m_animator.SetModel(m_model);
    }

    void Renderer::SetModel(const char* key)
    {
        if (ResourceRef<Model> model = ResourceManager::GetResource<Model>(key))
            SetModel(std::move(model));
    }

    void Renderer::SetShader(ResourceRef<ShaderProgram>&& shader)
    {
        m_shader = std::forward<ResourceRef<ShaderProgram>>(shader);
    }

    void Renderer::SetShader(const char* key)
    {
        if (ResourceRef<ShaderProgram> shader = ResourceManager::GetResource<ShaderProgram>(key))
            SetShader(std::move(shader));
    }

    void Renderer::SerializeText(std::ostream& output, EntityHandle owner,
                                 uint64 index) const
    {
        const std::string* model = ResourceManager::FindKeyByVal(m_model);
        const std::string* shader = ResourceManager::FindKeyByVal(m_shader);
        output << "[Renderer]\n    ";

        if constexpr (UpdateAfterParent<Renderer>::m_value)
        {
            text::Serialize(output, "index", index);
            output << "\n    ";
        }

        text::Serialize(output, "owner", owner);
        output << "\n    ";
        text::Serialize(output, "model", (model) ? *model : "");
        output << "\n    ";
        text::Serialize(output, "shader", (shader) ? *shader : "");
        output << "\n        ";
        text::Serialize(output, "vertexShader", (m_shader) ? m_shader->GetVertexShaderName() : "");
        output << "\n        ";
        text::Serialize(output, "fragmentShader", (m_shader) ? m_shader->GetFragmentShaderName() : "");
        output << "\n    ";

        std::vector<const std::string*> validMeshMaterials;
        validMeshMaterials.reserve(m_materials.size());

        for (const ResourceRef<MeshMaterial>& mat : m_materials)
        {
            if (mat)
                validMeshMaterials.push_back(ResourceManager::FindKeyByVal(mat));
        }

        text::Serialize(output, "materialCount", validMeshMaterials.size());
        output << "\n    ";

        for (const std::string* matKey : validMeshMaterials)
        {
            text::Serialize(output, "material", *matKey);
            output << "\n    ";
        }
        text::Serialize(output, "flags", m_flags);
        output << '\n';
    }

    SkeletonAnimator& Renderer::GetAnimator(void)
    {
        return m_animator;
    }

    const char* Renderer::DeserializeText(const char* text, const char* end)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, m_owner);

        std::string key;
        text = text::DeserializeString(text, end, key);

        if (!key.empty())
        {
            ResourceManager::Load<Model>(key);
            m_model = ResourceManager::GetResource<Model>(key);
            m_animator.SetModel(m_model);
        }

        key.clear();
        text = text::DeserializeString(text, end, key);

        std::string vertShader, fragShader;

        text = text::DeserializeString(text, end, vertShader);
        text = text::DeserializeString(text, end, fragShader);

        if (!key.empty() && !vertShader.empty() && !fragShader.empty())
        {
            ResourceManager::LoadShader(key.c_str(), vertShader.c_str(), fragShader.c_str(), true);
            m_shader = ResourceManager::GetResource<ShaderProgram>(key);
        }


        uint64 matCount = 0;
        std::string material;

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, matCount);

        m_materials.resize(matCount);

        for (uint64 matNum = 0; matNum < matCount; ++matNum)
        {
            material.clear();
            text = text::DeserializeString(text, end, material);
            ResourceManager::Load<MeshMaterial>(material);
            m_materials[matNum] = ResourceManager::GetResource<MeshMaterial>(material);
        }

        MOVE_TEXT_CURSOR(text, end);
        return text::DeserializeInteger(text, m_flags);
    }

}