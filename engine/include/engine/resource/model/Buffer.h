#pragma once

#include "CoreTypes.h"

#include <vector>


namespace engine
{
	struct Vertex;

	class Buffer
	{
	public:
					Buffer(void);
					~Buffer(void);

		uint32		GetBufferID(void) const noexcept;
		void		SetData(Vertex const* vertexData, uint64 size) const;
		void		SetData(int32 const* indexData, uint64 size) const;
		void		DeleteData(void);

	private:
		uint32		m_buffer;
	};
}