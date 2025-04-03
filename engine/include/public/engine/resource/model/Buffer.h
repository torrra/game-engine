#pragma once

#include "engine/CoreTypes.h"

#include <vector>


namespace engine
{
	struct Vertex;

	class Buffer
	{
	public:
					Buffer(void);
                    Buffer(int32);
					~Buffer(void);

		uint32		GetBufferID(void) const noexcept;
		void		SetData(void* data, uint64 size);
		void		DeleteData(void);

	private:

		uint32		m_buffer;

        friend class Mesh;
	};
}