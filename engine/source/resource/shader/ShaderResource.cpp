#include "resource/shader/ShaderResource.h"
#include "utility/FileData.h"

#include <glad/glad.h>

#include "utility/MemoryCheck.h"

engine::Shader::Shader(void)
	: m_shader(0), m_shaderType(EShaderType::INVALID_SHADER)
{
}

void engine::Shader::LoadResource(const char* fileName)
{
	// Check shader type (vert, frag, etc...)
	m_shaderType = ShaderType(fileName);

	if (m_shaderType == EShaderType::INVALID_SHADER)
		return;

	// Load shader file to memory
	FileData fileData = fileData.ReadFile(fileName);

	if (!fileData.m_size)
	{
		std::printf("Failed to process shader '%s'. Shader file contains no content.\n", fileName);
		m_shaderType = EShaderType::INVALID_SHADER;
		return;
	}

	// Create shader
	m_shader = glCreateShader(m_shaderType);
	// TODO: check if length (last param) needs to be nullptr
	glShaderSource(m_shader, 1, &fileData.m_fileContent, &fileData.m_size);
	glCompileShader(m_shader);

	int32 result;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		const int32 infoLogLength = 2500;
		char infoLog[infoLogLength];

		glGetShaderInfoLog(m_shader, infoLogLength, nullptr, infoLog);
		std::printf("Failed to compile shader. Error: %s", infoLog);
		m_shaderType = EShaderType::INVALID_SHADER;

		return;
	}

	fileData.Clear();
}

uint32 engine::Shader::GetShader(void) const noexcept
{
	return m_shader;
}

engine::EShaderType engine::Shader::GetShaderType(void) const noexcept
{
	return m_shaderType;
}

engine::EShaderType engine::Shader::ShaderType(const char* fileName) const
{
	uint64 pos = std::string(fileName).rfind('.');

	if (pos == std::string::npos)
	{
		std::printf("Failed to process shader. Shader file does not have an extension.\n");
		return EShaderType::INVALID_SHADER;
	}
	
	switch (fileName[pos + 1])
	{
	case 'v':
		return EShaderType::VERTEX_SHADER;
	case 'f':
		return EShaderType::FRAGMENT_SHADER;
	case 'g':
		return EShaderType::GEOMETRY_SHADER;
	default:
		return EShaderType::INVALID_SHADER;
	}
}
