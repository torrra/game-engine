#pragma once

#include "engine/resource/Resource.h"
#include "engine/EngineExport.h"
#include "Mesh.h"

#include <vector>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace engine
{
	struct Vertex;
	class Buffer;

	class Model final : public IResource
	{
	public:
		ENGINE_API				Model(void) = default;
		ENGINE_API virtual		~Model(void) = default;

		ENGINE_API virtual void	LoadResource(const char* fileName) override;

	private:
		void					ProcessNodes(aiNode* node, const aiScene* scene);
		
		std::vector<Mesh>	m_meshes;
	};
}