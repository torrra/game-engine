#pragma once

#include "resource/Resource.h"
#include "engine/CoreTypes.h"

namespace engine
{
	enum EShaderType : uint16
	{
		INVALID_SHADER,
		VERTEX_SHADER = 0x8B31,
		FRAGMENT_SHADER = 0x8B30,
		GEOMETRY_SHADER = 0x8DD9
	};

	class Shader : public IResource
	{
	public:
		Shader(void);
		~Shader(void) = default;

		virtual void LoadResource(const char* fileName) override;

		uint32 GetShader(void) const noexcept;
		EShaderType GetShaderType(void) const noexcept;

	private:
		EShaderType ShaderType(const char* fileName) const;
		uint32 m_shader;
		EShaderType m_shaderType;
	};
}