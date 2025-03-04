#include "model/Buffer.h"
#include "model/Vertex.h"

#include <glad/glad.h>

engine::Buffer::Buffer(void)
	: m_buffer(0)
{
	glCreateBuffers(1, &m_buffer);
}

engine::Buffer::~Buffer(void)
{
	DeleteData();
}

uint32_t engine::Buffer::GetBufferID(void) const noexcept
{
	return m_buffer;
}

void engine::Buffer::SetData(Vertex const* vertexData, size_t size) const
{
	glNamedBufferData(m_buffer, size, vertexData, GL_STATIC_DRAW);
}

void engine::Buffer::SetData(int32_t const* indexData, size_t size) const
{
	glNamedBufferData(m_buffer, size, indexData, GL_STATIC_DRAW);
}

void engine::Buffer::DeleteData(void)
{
	glDeleteBuffers(1, &m_buffer);

	m_buffer = 0;
}
