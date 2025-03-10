#pragma once

#include "Vertex.h"
#include "engine/CoreTypes.h"

#include <vector>

struct aiMesh;

template <typename TReal>
class aiVector3t;

namespace engine
{
	class Buffer;

	class Mesh
	{
	public:
					Mesh(void) = delete;
					Mesh(const aiMesh* mesh);
					~Mesh(void) = default;

		void		SetupBuffers(void);

		Mesh&		operator=(const aiMesh* mesh);
	private:
		uint32		SetAttributes(void);
		void		SetAttribute(uint32& index, int32 size, uint32& stride) const;
		Buffer		CreateVBO(void);
		Buffer		CreateEBO(void);
		void		PostLoad(void);

		math::Vector2<float>	ConvertVec2(aiVector3t<float> const& vec3) const noexcept;
		math::Vector3<float>	ConvertVec3(aiVector3t<float> const& vec3) const noexcept;
	public:
		std::vector<Vertex>		m_vertices;
		std::vector<int32>		m_indices;
		uint32					m_indexCount;
		uint32					m_vao;
	};
}