#pragma once

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
	class ShaderProgram
	{
	public:
		ENGINE_API ShaderProgram(void) = delete;
		ENGINE_API ShaderProgram(const char* vertexShader, const char* fragShader);
		ENGINE_API ~ShaderProgram(void) = default;

		ENGINE_API void Use(void) const;

		// Scalar types
		ENGINE_API void Set(const char* uniformName, bool value);
		ENGINE_API void Set(const char* uniformName, int value);
		ENGINE_API void Set(const char* uniformName, uint32 value);
		ENGINE_API void Set(const char* uniformName, float value);
		ENGINE_API void Set(const char* uniformName, double value);

		// Vectors
		ENGINE_API void Set(const char* uniformName, math::Vector2<int> const& vec);
		ENGINE_API void Set(const char* uniformName, math::Vector3<int> const& vec);
		ENGINE_API void Set(const char* uniformName, math::Vector4<int> const& vec);

		ENGINE_API void Set(const char* uniformName, math::Vector2<float> const& vec);
		ENGINE_API void Set(const char* uniformName, math::Vector3<float> const& vec);
		ENGINE_API void Set(const char* uniformName, math::Vector4<float> const& vec);

		ENGINE_API void Set(const char* uniformName, math::Vector2<double> const& vec);
		ENGINE_API void Set(const char* uniformName, math::Vector3<double> const& vec);
		ENGINE_API void Set(const char* uniformName, math::Vector4<double> const& vec);

		// Matrices
		ENGINE_API void Set(const char* uniformName, math::Matrix2<float>* matrix);
		ENGINE_API void Set(const char* uniformName, math::Matrix3<float>* matrix);
		ENGINE_API void Set(const char* uniformName, math::Matrix4<float>* matrix);

		ENGINE_API void Set(const char* uniformName, math::Matrix2<double>* matrix);
		ENGINE_API void Set(const char* uniformName, math::Matrix3<double>* matrix);
		ENGINE_API void Set(const char* uniformName, math::Matrix4<double>* matrix);

	private:
		void CreateProgram(void);

		std::string m_vertexShader;
		std::string m_fragShader;
		
		uint32 m_programID;
	};
}