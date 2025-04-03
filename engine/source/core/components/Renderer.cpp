#include <math/Matrix4.hpp>

#include "core/components/Renderer.h"
#include "core/components/Transform.h"

#include "resource/ResourceManager.h"
#include "resource/model/Model.h"
#include "resource/shader/Shader.h"
#include "resource/texture/Texture.h"

#include "core/SceneGraph.h"

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

        if (m_texture)
        {
            m_texture->UseTexture();
        }
        else
            Texture::RemoveTexture();

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

        DrawModel();
    }

    const Model* Renderer::GetModel(void) const
    {
        return m_model;
    }

    const ShaderProgram* Renderer::GetShader(void) const
    {
        return m_shader;
    }

    const Texture* Renderer::GetTexture(void) const
    {
        return m_texture;
    }

    void Renderer::SetModel(const Model* model)
    {
        m_model = model;
    }

    void Renderer::SetModel(const char* key)
    {
        if (const Model* model = ResourceManager::GetResource<Model>(key))
            m_model = model;
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

    void Renderer::SetTexture(const Texture* texture)
    {
        m_texture = texture;
    }

    void Renderer::SetTexture(const char* key)
    {
        if (const Texture* texture = ResourceManager::GetResource<Texture>(key))
            m_texture = texture;
    }

    void Renderer::DrawModel(void) const
    {
        if (m_model->IsDynamic())
        {
            for (const DynamicMesh& mesh : m_model->GetDynamicMeshes())
            {
                glBindVertexArray(mesh.GetVertexArrayID());
                glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
        else
        {
            for (const Mesh& mesh : m_model->GetStaticMeshes())
            {
                glBindVertexArray(mesh.GetVertexArrayID());
                glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }



}