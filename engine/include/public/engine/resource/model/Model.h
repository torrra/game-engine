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
		
		// This function is meant to be used by renderer components, and ONLY from the
		// render thread.
		ENGINE_API
		const std::vector<Mesh>& GetMeshes(void) const;

		mutable std::vector<Mesh>			m_meshes;
		mutable uint8						m_loadStatus = 0;

		friend class Renderer;
	};
}