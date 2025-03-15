#include "resource/model/Model.h"
#include "resource/model/Buffer.h"
#include "resource/model/Vertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void engine::Model::LoadResource(const char* fileName)
{
	// Read file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs | aiProcess_CalcTangentSpace
	);

	// Error management
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::printf("Failed to load model '%s'. Error: %s\n", fileName, importer.GetErrorString());
		return;
	}
	
	ProcessNodes(scene->mRootNode, scene);
	
	for (Mesh& mesh : m_meshes)
	{
		mesh.SetupBuffers();
	}
}

void engine::Model::Update(void)
{
	for (Mesh& mesh : m_meshes)
	{
		glBindVertexArray(mesh.m_vao);
		glDrawElements(GL_TRIANGLES, mesh.m_indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

const std::vector<engine::Mesh>& engine::Model::GetMeshes(void) const
{
	return m_meshes;
}

void engine::Model::ProcessNodes(aiNode* node, const aiScene* scene)
{
	m_meshes.reserve(scene->mNumMeshes);

	// Process current node's mesh(es)
	for (uint32 i = 0; i < node->mNumMeshes; ++i)
	{
		Mesh meshData = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(meshData);
	}

	// Handle child nodes
	for (uint32 i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNodes(node->mChildren[i], scene);
	}
}