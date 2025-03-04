#include "resource/shader/Shader.h"
#include "resource/shader/ShaderResource.h"
#include "resource/ResourceManager.h"

#include <glad/glad.h>

engine::ShaderProgram::ShaderProgram(const char* vertexShader, const char* fragShader)
	: m_vertexShader(vertexShader), m_fragShader(fragShader), m_programID(0)
{
	CreateProgram();
}

ENGINE_API void engine::ShaderProgram::Use(void) const
{
	return glUseProgram(m_programID);
}

// Scalar types
ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, bool value)
{
	glUniform1i(glGetUniformLocation(m_programID, uniformName), value);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, int value)
{
	glUniform1i(glGetUniformLocation(m_programID, uniformName), value);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, uint32_t value)
{
	glUniform1ui(glGetUniformLocation(m_programID, uniformName), value);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, float value)
{
	glUniform1f(glGetUniformLocation(m_programID, uniformName), value);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, double value)
{
	glUniform1d(glGetUniformLocation(m_programID, uniformName), value);
}

// Vector ints
ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector2<int> const& vec)
{
	glUniform2i(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1]);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector3<int> const& vec)
{
	glUniform3i(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[3]);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector4<int> const& vec)
{
	glUniform4i(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[3], vec[4]);
}

// Vector floats
ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector2<float> const& vec)
{
	glUniform2f(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1]);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector3<float> const& vec)
{
	glUniform3f(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[3]);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector4<float> const& vec)
{
	glUniform4f(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[3], vec[4]);
}

// Vector double
ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector2<double> const& vec)
{
	glUniform2d(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1]);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector3<double> const& vec)
{
	glUniform3d(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[3]);
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Vector4<double> const& vec)
{
	glUniform4d(glGetUniformLocation(m_programID, uniformName), vec[0], vec[1], vec[3], vec[4]);
}

// Matrix float
ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Matrix2<float>* matrix)
{
	int location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix2fv(location, 1, GL_FALSE, reinterpret_cast<float*>(matrix));
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Matrix3<float>* matrix)
{
	int location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix3fv(location, 1, GL_FALSE, reinterpret_cast<float*>(matrix));
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Matrix4<float>* matrix)
{
	int location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<float*>(matrix));
}

// Matrix double
ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Matrix2<double>* matrix)
{
	int location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix2dv(location, 1, GL_FALSE, reinterpret_cast<double*>(matrix));
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Matrix3<double>* matrix)
{
	int location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix3dv(location, 1, GL_FALSE, reinterpret_cast<double*>(matrix));
}

ENGINE_API void engine::ShaderProgram::Set(const char* uniformName, math::Matrix4<double>* matrix)
{
	int location = glGetUniformLocation(m_programID, uniformName);

	glUniformMatrix4dv(location, 1, GL_FALSE, reinterpret_cast<double*>(matrix));
}

void engine::ShaderProgram::CreateProgram(void)
{
	// Resource manager will only load if resource does not exist
	ResourceManager::Load<Shader>(m_vertexShader);
	ResourceManager::Load<Shader>(m_fragShader);
	
	Shader* vShader = ResourceManager::GetResource<Shader>(m_vertexShader);
	Shader* fShader = ResourceManager::GetResource<Shader>(m_fragShader);

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

	int result;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);

	if (!result)
	{
		constexpr int bufferSize = 2500;
		char infoLog[bufferSize];

		glGetProgramInfoLog(m_programID, bufferSize, nullptr, infoLog);
		std::printf("Failed to link shader program, reason: %s\n", infoLog);

		return;
	}

	glDeleteShader(vShader->GetShader());
	glDeleteShader(fShader->GetShader());

}
