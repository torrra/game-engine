
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

	uint32 stride = SetAttributes();

	glVertexArrayElementBuffer(m_vao, ebo.GetBufferID());
	glVertexArrayVertexBuffer(m_vao, 0, vbo.GetBufferID(), 0, stride);

	PostLoad();
}

uint32 engine::Mesh::SetAttributes(void)
{
	uint32 stride = 0;
	uint32 index = 0;

	// Position
	SetAttribute(index, 3, stride);

	// Normal
	SetAttribute(index, 3, stride);

	// Tangent
	SetAttribute(index, 3, stride);

	// BiTangent
	SetAttribute(index, 3, stride);

	// TexCoord
	SetAttribute(index, 2, stride);

	return stride;
}

void engine::Mesh::SetAttribute(uint32& index, int32 size, uint32& stride) const
{
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
	uint64 size = m_vertices.size() * sizeof(Vertex);

	// Set data
	vbo.SetData(m_vertices.data(), size);

	return vbo;
}

engine::Buffer engine::Mesh::CreateEBO(void)
{
	Buffer ebo;

	// Size
	uint64 size = m_indices.size() * sizeof(uint32);

	// Set data
	ebo.SetData(m_indices.data(), size);

	return ebo;
}

void engine::Mesh::PostLoad(void)
{
	m_indexCount = static_cast<uint32>(m_indices.size());

	m_indices.clear();
	m_vertices.clear();

	m_indices.shrink_to_fit();
	m_vertices.shrink_to_fit();
}

engine::Mesh& engine::Mesh::operator=(const aiMesh* mesh)
{
	// Vertices
	for (uint32 i = 0; i < mesh->mNumVertices; ++i)
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

math::Vector2<float> engine::Mesh::ConvertVec2(aiVector3t<float> const& vec3) const noexcept
{
	return math::Vector2<float>(
		static_cast<float>(vec3.x),
		static_cast<float>(vec3.y)
	);
}

math::Vector3<float> engine::Mesh::ConvertVec3(aiVector3t<float> const& vec3) const noexcept
{
	return math::Vector3<float>(vec3.x, vec3.y, vec3.z);
}
