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

	void Renderer::Render(const math::Matrix4f& viewProjection)
	{
		if (!m_model || !m_shader)
			return;

		if (m_texture)
		{
			m_texture->UseTexture();
		}
		else
			Texture::RemoveTexture();

		m_shader->Use();

		if (Transform* transform = m_currentScene->GetComponent<Transform>(m_owner))
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

	const Model* Renderer::GetModel(void)
	{
		return m_model;
	}

	const ShaderProgram* Renderer::GetShader(void)
	{
		return m_shader;
	}

	const Texture* Renderer::GetTexture(void)
	{
		return m_texture;
	}

	void Renderer::SetModel(Model* model)
	{
		m_model = model;
	}

	void Renderer::SetModel(const char* key)
	{
		if (Model* model = ResourceManager::GetResource<Model>(key))
			m_model = model;
	}

	void Renderer::SetShader(ShaderProgram* shader)
	{
		m_shader = shader;
	}

	void Renderer::SetShader(const char* key)
	{
		if (ShaderProgram* shader = ResourceManager::GetResource<ShaderProgram>(key))
			m_shader = shader;
	}

	void Renderer::SetTexture(Texture* texture)
	{
		m_texture = texture;
	}

	void Renderer::SetTexture(const char* key)
	{
		if (Texture* texture = ResourceManager::GetResource<Texture>(key))
			m_texture = texture;
	}

	void Renderer::DrawModel(void)
	{
		for (const Mesh& mesh : m_model->GetMeshes())
		{
			glBindVertexArray(mesh.m_vao);
			glDrawElements(GL_TRIANGLES, mesh.m_indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}



}