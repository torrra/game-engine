#include "resource/shader/Shader.h"
#include "resource/shader/ShaderResource.h"
#include "resource/ResourceManager.h"
#include "utility/MemoryCheck.h"
#include "ConsoleLog.hpp"

#include <glad/glad.h>

engine::ShaderProgram::ShaderProgram(const char* vertexShader, const char* fragShader)
	: m_vertexShader(vertexShader), m_fragShader(fragShader), m_programID(0)
{
	CreateProgram(false, false);
}

bool engine::ShaderProgram::LoadResource(const char* filePath)
{
	(void) filePath;
    return true;
}

void engine::ShaderProgram::Use(void) const
{
	return glUseProgram(m_programID);
}

// Scalar types
void engine::ShaderProgram::Set(const char* uniformName, bool value) const
{
	glUniform1i(glGetUniformLocation(m_programID, uniformName), value);
}

void engine::ShaderProgram::Set(const char* uniformName, int32 value) const
{
	glUniform1i(glGetUniformLocation(m_programID, uniformName), value);
}

void engine::ShaderProgram::Set(const char* uniformName, uint32 value) const
{
	glUniform1ui(glGetUniformLocation(m_programID, uniformName), value);
}

void engine::ShaderProgram::Set(const char* uniformName, f32 value) const
{
	glUniform1f(glGetUniformLocation(m_programID, uniformName), value);
}
 
void engine::ShaderProgram::Set(const char* uniformName, f64 value) const
{
	glUniform1d(glGetUniformLocation(m_programID, uniformName), value);
}

// Vector ints
void engine::ShaderProgram::Set(const char* uniformName, math::Vector2i const& vec) const
{
	glUniform2i(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1]);
}

void engine::ShaderProgram::Set(const char* uniformName, math::Vector3i const& vec) const
{
	glUniform3i(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[2]);
}

void engine::ShaderProgram::Set(const char* uniformName, math::Vector4i const& vec) const
{
	glUniform4i(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[2], vec[3]);
}

// Vector floats
void engine::ShaderProgram::Set(const char* uniformName, math::Vector2f const& vec) const
{
	glUniform2f(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1]);
}

void engine::ShaderProgram::Set(const char* uniformName, math::Vector3f const& vec) const
{
	glUniform3f(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[2]);
}

void engine::ShaderProgram::Set(const char* uniformName, math::Vector4f const& vec) const
{
	glUniform4f(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[2], vec[3]);
}

// Vector double
void engine::ShaderProgram::Set(const char* uniformName, math::Vector2d const& vec) const
{
	glUniform2d(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1]);
}

void engine::ShaderProgram::Set(const char* uniformName, math::Vector3d const& vec) const
{
	glUniform3d(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[2]);
}

void engine::ShaderProgram::Set(const char* uniformName, math::Vector4d const& vec) const
{
	glUniform4d(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[2], vec[3]);
}

// Matrix float
void engine::ShaderProgram::Set(const char* uniformName, const math::Matrix2f* matrix) const
{
	int32 location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix2fv(location, 1, GL_FALSE, reinterpret_cast<const f32*>(matrix));
}

void engine::ShaderProgram::Set(const char* uniformName, const math::Matrix3f* matrix) const
{
	int32 location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix3fv(location, 1, GL_FALSE, reinterpret_cast<const f32*>(matrix));
}

void engine::ShaderProgram::Set(const char* uniformName, const math::Matrix4f* matrix) const
{
	int32 location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const f32*>(matrix));
}

// Matrix double
void engine::ShaderProgram::Set(const char* uniformName, const math::Matrix2d* matrix) const
{
	int32 location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix2dv(location, 1, GL_FALSE, reinterpret_cast<const f64*>(matrix));
}

void engine::ShaderProgram::Set(const char* uniformName, const math::Matrix3d* matrix) const
{
	int32 location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix3dv(location, 1, GL_FALSE, reinterpret_cast<const f64*>(matrix));
}

void engine::ShaderProgram::Set(const char* uniformName, const math::Matrix4d* matrix) const
{
	int32 location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix4dv(location, 1, GL_FALSE, reinterpret_cast<const f64*>(matrix));
}

const std::string& engine::ShaderProgram::GetVertexShaderName(void) const
{
    return m_vertexShader;
}

const std::string& engine::ShaderProgram::GetFragmentShaderName(void) const
{
    return m_fragShader;
}

void engine::ShaderProgram::CreateProgram(bool isVertAbsolute, bool isFragAbsolute)
{
    // Don't create OpenGL program twice
    if (m_programID)
        return;
     
	// Resource manager will only load if resource does not exist
	ResourceManager::Load<Shader>(m_vertexShader, isVertAbsolute);
	ResourceManager::Load<Shader>(m_fragShader, isFragAbsolute);
	
	const Shader* vShader = ResourceManager::GetResource<Shader>(m_vertexShader);
	const Shader* fShader = ResourceManager::GetResource<Shader>(m_fragShader);

    if (!vShader || !fShader)
    {
        PrintLog(ErrorPreset(), "Failed to compute shader");
        return;
    }

	if (!vShader->GetShaderType() ||
		!fShader->GetShaderType())
	{
		std::printf("Failed to create shader program. Error: vertex or fragment shader error.\n");
		return;
	}
	
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vShader->GetShader());
	glAttachShader(m_programID, fShader->GetShader());
	glLinkProgram(m_programID);

	int32 result;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);

	if (!result)
	{
		constexpr int32 bufferSize = 2500;
		char infoLog[bufferSize];

		glGetProgramInfoLog(m_programID, bufferSize, nullptr, infoLog);
		std::printf("Failed to link shader program, reason: %s\n", infoLog);

		return;
	}

	glDeleteShader(vShader->GetShader());
	glDeleteShader(fShader->GetShader());

}
