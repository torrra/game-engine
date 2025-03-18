#include "resource/model/Model.h"
#include "resource/model/Buffer.h"
#include "resource/model/Vertex.h"

#include "engine/thread/ThreadManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

void engine::Model::LoadResource(const char* fileName)
{
	ThreadManager::AddTask([this, fileName = std::string(fileName)]
		{
			// Read file
			Assimp::Importer importer;
			const aiScene * scene = importer.ReadFile(fileName,
				aiProcess_Triangulate | aiProcess_GenSmoothNormals |
				aiProcess_FlipUVs | aiProcess_CalcTangentSpace
			);

			// Error management
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::printf("Failed to load model '%s'. Error: %s\n", fileName.c_str(), importer.GetErrorString());
				return;
			}

			ProcessNodes(scene->mRootNode, scene);
			m_loadStatus |= LOADED;
		}
	);
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
	// If model has finished loading from another thread but has not yet
	// called the OpenGL functions to setup its buffers, do it now before
	// passing meshes over to caller
	if ((m_loadStatus & LOADED) && !(m_loadStatus & GRAPHICS_CALLS_COMPLETE))
	{
		for (Mesh& mesh : m_meshes)
		{
			mesh.SetupBuffers();
		}

		m_loadStatus |= GRAPHICS_CALLS_COMPLETE;
	}

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