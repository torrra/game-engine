#pragma once

#include <stdint.h>
#include <vector>

namespace engine
{
	struct Vertex;

	class Buffer
	{
	public:
					Buffer(void);
					~Buffer(void);

		uint32_t	GetBufferID(void) const noexcept;
		void		SetData(Vertex const* vertexData, size_t size) const;
		void		SetData(int32_t const* indexData, size_t size) const;
		void		DeleteData(void);

	private:
		uint32_t	m_buffer;
	};
}