#pragma once

#include "Vertex.h"

#include <vector>

struct aiMesh;

template <typename TReal>
class aiVector3t;
//typedef float ai_real;
//typedef aiVector3t<ai_real> aiVector3D;

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
		uint32_t	SetAttributes(void);
		void		SetAttribute(int32_t size, uint32_t& stride) const;
		Buffer		CreateVBO(void);
		Buffer		CreateEBO(void);
		void		PostLoad(void);

		math::Vector2<float>	ConvertVec2(aiVector3t<float> const& vec3) const noexcept;
		math::Vector3<float>	ConvertVec3(aiVector3t<float> const& vec3) const noexcept;
	public:
		std::vector<Vertex>		m_vertices;
		std::vector<int32_t>	m_indices;
		uint32_t				m_indexCount;
		uint32_t				m_vao;
	};
}