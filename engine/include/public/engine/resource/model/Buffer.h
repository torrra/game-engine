#pragma once

#include "engine/CoreTypes.h"

#include <vector>


namespace engine
{
	class Buffer
	{
	public:
					Buffer(void);
                    Buffer(int32);
                    Buffer(const Buffer&) = default;
                    Buffer(Buffer&&) noexcept = default;
					~Buffer(void) = default;

        void        Init(void);
		uint32		GetBufferID(void) const noexcept;
		void		SetData(void* data, uint64 size);
		void		SetData(void* data, uint64 size, uint32 offset);
		void		DeleteData(void);

        Buffer& operator=(Buffer&&) noexcept = default;

	private:

		uint32		m_buffer;
	};
}