#pragma once

#include "resource/Resource.h"
#include "Mesh.h"

#include <vector>
#include "EngineExport.h"

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
		void			Update(void); // TODO: temporary function remove

	private:
		void			ProcessNodes(aiNode* node, const aiScene* scene);
		
		std::vector<Mesh>	m_meshes;
	};
}