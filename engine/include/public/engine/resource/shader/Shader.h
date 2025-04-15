#pragma once

#include "engine/resource/Resource.h"
#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>
#include <math/Vector3.hpp>
#include <math/Vector4.hpp>
#include <math/Matrix2.hpp>
#include <math/Matrix3.hpp>
#include <math/Matrix4.hpp>

#include <string>

namespace engine
{
	class ShaderProgram final : public IResource
	{
	public:
		ENGINE_API ShaderProgram(void) = default;
		ENGINE_API ShaderProgram(const char* vertexShader, const char* fragShader);
		ENGINE_API virtual ~ShaderProgram(void) override = default;

		bool LoadResource(const char* filePath) override;

		ENGINE_API void Use(void) const;

		// Scalar types
		ENGINE_API void Set(const char* uniformName, bool value) const;
		ENGINE_API void Set(const char* uniformName, int32 value) const;
		ENGINE_API void Set(const char* uniformName, uint32 value) const;
		ENGINE_API void Set(const char* uniformName, f32 value) const;
		ENGINE_API void Set(const char* uniformName, f64 value) const;

		// Vectors
		ENGINE_API void Set(const char* uniformName, math::Vector2i const& vec) const;
		ENGINE_API void Set(const char* uniformName, math::Vector3i const& vec) const;
		ENGINE_API void Set(const char* uniformName, math::Vector4i const& vec) const;

		ENGINE_API void Set(const char* uniformName, math::Vector2f const& vec) const;
		ENGINE_API void Set(const char* uniformName, math::Vector3f const& vec) const;
		ENGINE_API void Set(const char* uniformName, math::Vector4f const& vec) const;

		ENGINE_API void Set(const char* uniformName, math::Vector2d const& vec) const;
		ENGINE_API void Set(const char* uniformName, math::Vector3d const& vec) const;
		ENGINE_API void Set(const char* uniformName, math::Vector4d const& vec) const;

		// Matrices
		ENGINE_API void Set(const char* uniformName, math::Matrix2f* matrix) const;
		ENGINE_API void Set(const char* uniformName, math::Matrix3f* matrix) const;
		ENGINE_API void Set(const char* uniformName, math::Matrix4f* matrix) const;

		ENGINE_API void Set(const char* uniformName, math::Matrix2d* matrix) const;
		ENGINE_API void Set(const char* uniformName, math::Matrix3d* matrix) const;
		ENGINE_API void Set(const char* uniformName, math::Matrix4d* matrix) const;

	private:
		void CreateProgram(void);

		std::string m_vertexShader;
		std::string m_fragShader;
		
		uint32 m_programID = 0;

        friend class ResourceManager;
	};
}