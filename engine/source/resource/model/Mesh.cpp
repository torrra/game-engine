#include "resource/model/Mesh.h"
#include "resource/model/Buffer.h"

#include <assimp/mesh.h>
#include <assimp/vector3.h>

#include <glad/glad.h>

engine::Mesh::Mesh(const aiMesh* mesh)
	: m_indexCount(mesh->mNumFaces), m_vao(0)
{
	*this = mesh;
}

void engine::Mesh::SetupBuffers(void)
{
	glCreateVertexArrays(1, &m_vao);
	Buffer vbo = CreateVBO();
	Buffer ebo = CreateEBO();

	uint32_t stride = SetAttributes();

	glVertexArrayElementBuffer(m_vao, ebo.GetBufferID());
	glVertexArrayVertexBuffer(m_vao, 0, vbo.GetBufferID(), 0, stride);

	PostLoad();
}

uint32_t engine::Mesh::SetAttributes(void)
{
	uint32_t stride = 0;

	// Position
	SetAttribute(3, stride);

	// Normal
	SetAttribute(3, stride);

	// Tangent
	SetAttribute(3, stride);

	// BiTangent
	SetAttribute(3, stride);

	// TexCoord
	SetAttribute(2, stride);

	return stride;
}

void engine::Mesh::SetAttribute(int32_t size, uint32_t& stride) const
{
	static uint32_t index = 0;

	glEnableVertexArrayAttrib(m_vao, index);
	glVertexArrayAttribFormat(m_vao, index, size, GL_FLOAT, GL_FALSE, stride);
	glVertexArrayAttribBinding(m_vao, index, 0);

	++index;
	stride += size * sizeof(float);
}

engine::Buffer engine::Mesh::CreateVBO(void)
{
	Buffer vbo;

	// Size
	size_t size = m_vertices.size() * sizeof(Vertex);

	// Set data
	vbo.SetData(m_vertices.data(), size);

	return vbo;
}

engine::Buffer engine::Mesh::CreateEBO(void)
{
	Buffer ebo;

	// Size
	size_t size = m_indices.size() * sizeof(uint32_t);

	// Set data
	ebo.SetData(m_indices.data(), size);

	return ebo;
}

void engine::Mesh::PostLoad(void)
{
	m_indexCount = static_cast<uint32_t>(m_indices.size());

	m_indices.clear();
	m_vertices.clear();

	m_indices.shrink_to_fit();
	m_vertices.shrink_to_fit();
}

engine::Mesh& engine::Mesh::operator=(const aiMesh* mesh)
{
	// Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex{};

		// Position
		vertex.m_position = ConvertVec3(mesh->mVertices[i]);

		// Normal
		if (mesh->HasNormals())
			vertex.m_normal = ConvertVec3(mesh->mNormals[i]);

		// TexCoords
		if (mesh->mTextureCoords[0])
			vertex.m_texCoord = ConvertVec2(mesh->mTextureCoords[0][i]);

		// Tangent / biTangents
		if (mesh->HasTangentsAndBitangents())
		{
			vertex.m_tangent = ConvertVec3(mesh->mTangents[i]);
			vertex.m_biTangent = ConvertVec3(mesh->mBitangents[i]);
		}

		m_vertices.push_back(vertex);
	}

	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			m_indices.push_back(face.mIndices[j]);
		}
	}

	return *this;
}

math::Vector2<int> engine::Mesh::ConvertVec2(aiVector3t<float> const& vec3) const noexcept
{
	return math::Vector2<int>(
		static_cast<int>(vec3.x),
		static_cast<int>(vec3.y)
	);
}

math::Vector3<float> engine::Mesh::ConvertVec3(aiVector3t<float> const& vec3) const noexcept
{
	return math::Vector3<float>(vec3.x, vec3.y, vec3.z);
}
