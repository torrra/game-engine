#include "resource/model/Buffer.h"
#include "resource/model/Vertex.h"

#include <glad/glad.h>
#include "resource/model/Buffer.h"

engine::Buffer::Buffer(void)
	: m_buffer(0)
{
	glCreateBuffers(1, &m_buffer);
}

engine::Buffer::Buffer(int32)
    : m_buffer(0)
{
}


void engine::Buffer::Init(void)
{
    if (!m_buffer)
        glCreateBuffers(1, &m_buffer);
}

uint32 engine::Buffer::GetBufferID(void) const noexcept
{
	return m_buffer;
}

void engine::Buffer::SetData(void* data, uint64 size)
{
	glNamedBufferData(m_buffer, size, data, GL_STATIC_DRAW);
}

void engine::Buffer::DeleteData(void)
{
	glDeleteBuffers(1, &m_buffer);

	m_buffer = 0;
}
