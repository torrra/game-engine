#include <fstream>
#include <type_traits>
#include <iostream>

#include "engine/CoreTypes.h"

#include <math/Angle.hpp>
#include <math/Vector2.hpp>
#include <math/Vector3.hpp>
#include <math/Vector4.hpp>
#include <math/Quaternion.hpp>

namespace engine::text
{
	// Basic type is a non-pointer type that has a formatted I/O operator implementation
	template <typename TEvaluatedType>
	concept CSerializableType = requires(TEvaluatedType obj, std::ostream & os, std::istream & is)
	{
		os << obj;
		is >> obj;
	} && !std::is_pointer_v<TEvaluatedType>;

	template <CSerializableType TValueType>
	void Serialize(std::ofstream&, const char* name, TValueType);
}

namespace engine::text::types
{
	template <CSerializableType TValueType>
	std::string GetTypeName(void);
}

namespace engine::text
{
	template<CSerializableType TValueType>
	void Serialize(std::ofstream& file, const char* name, TValueType val)
	{
		const char* valName((name) ? name : "unnamed val");
		file << "@" <<  types::GetTypeName<TValueType>() << " | " << valName << '=' << val;
	}	
}


namespace engine::text::types
{
	template<CSerializableType TValueType>
	std::string GetTypeName(void)
	{
		return "unrecognized type";
	}

	template<>
	std::string GetTypeName<int32>(void)
	{
		return "int32";
	}

	template<>
	std::string GetTypeName<int64>(void)
	{
		return "int64";
	}

	template<>
	std::string GetTypeName<int16>(void)
	{
		return "int16";
	}

	template<>
	std::string GetTypeName<int8>(void)
	{
		return "int8";
	}

	template<>
	std::string GetTypeName<uint32>(void)
	{
		return "uint32";
	}

	template<>
	std::string GetTypeName<uint64>(void)
	{
		return "uint64";
	}

	template<>
	std::string GetTypeName<uint16>(void)
	{
		return "uint16";
	}

	template<>
	std::string GetTypeName<uint8>(void)
	{
		return "uint8";
	}

	template<>
	std::string GetTypeName<f32>(void)
	{
		return "f32";
	}

	template<>
	std::string GetTypeName<f64>(void)
	{
		return "f64";
	}


// Vector3

	template<>
	std::string GetTypeName<math::Vector3f>(void)
	{
		return "vec3f32";
	}

	template<>
	std::string GetTypeName<math::Vector3d>(void)
	{
		return "vec3f64";
	}


	template<>
	std::string GetTypeName<math::Vector3<uint32>>(void)
	{
		return "vec3ui32";
	}

	template<>
	std::string GetTypeName<math::Vector3<uint64>>(void)
	{
		return "vec3ui64";
	}

	template<>
	std::string GetTypeName<math::Vector3<uint16>>(void)
	{
		return "vec3ui16";
	}

	template<>
	std::string GetTypeName<math::Vector3<uint8>>(void)
	{
		return "vec3ui8";
	}

	template<>
	std::string GetTypeName<math::Vector3i>(void)
	{
		return "vec3i32";
	}

	template<>
	std::string GetTypeName<math::Vector3<int64>>(void)
	{
		return "vec3i64";
	}

	template<>
	std::string GetTypeName<math::Vector3<int16>>(void)
	{
		return "vec3i16";
	}

	template<>
	std::string GetTypeName<math::Vector3<int8>>(void)
	{
		return "vec3i8";
	}


// Vector2

	template<>
	std::string GetTypeName<math::Vector2f>(void)
	{
		return "vec2f32";
	}

	template<>
	std::string GetTypeName<math::Vector2d>(void)
	{
		return "vec2f64";
	}


	template<>
	std::string GetTypeName<math::Vector2<uint32>>(void)
	{
		return "vec2ui32";
	}

	template<>
	std::string GetTypeName<math::Vector2<uint64>>(void)
	{
		return "vec2ui64";
	}

	template<>
	std::string GetTypeName<math::Vector2<uint16>>(void)
	{
		return "vec2ui16";
	}

	template<>
	std::string GetTypeName<math::Vector2<uint8>>(void)
	{
		return "vec2ui8";
	}

	template<>
	std::string GetTypeName<math::Vector2i>(void)
	{
		return "vec2i32";
	}

	template<>
	std::string GetTypeName<math::Vector2<int64>>(void)
	{
		return "vec2i64";
	}

	template<>
	std::string GetTypeName<math::Vector2<int16>>(void)
	{
		return "vec2i16";
	}

	template<>
	std::string GetTypeName<math::Vector2<int8>>(void)
	{
		return "vec2i8";
	}


// Vector4

	template<>
	std::string GetTypeName<math::Vector4f>(void)
	{
		return "vec4f32";
	}

	template<>
	std::string GetTypeName<math::Vector4d>(void)
	{
		return "vec4f64";
	}


	template<>
	std::string GetTypeName<math::Vector4<uint32>>(void)
	{
		return "vec4ui32";
	}

	template<>
	std::string GetTypeName<math::Vector4<uint64>>(void)
	{
		return "vec4ui64";
	}

	template<>
	std::string GetTypeName<math::Vector4<uint16>>(void)
	{
		return "vec4ui16";
	}

	template<>
	std::string GetTypeName<math::Vector4<uint8>>(void)
	{
		return "vec4ui8";
	}

	template<>
	std::string GetTypeName<math::Vector4i>(void)
	{
		return "vec4i32";
	}

	template<>
	std::string GetTypeName<math::Vector4<int64>>(void)
	{
		return "vec4i64";
	}

	template<>
	std::string GetTypeName<math::Vector4<int16>>(void)
	{
		return "vec4i16";
	}

	template<>
	std::string GetTypeName<math::Vector4<int8>>(void)
	{
		return "vec4i8";
	}


// Radian / Degree

	template<>
	std::string GetTypeName<math::Radian<f32>>(void)
	{
		return "rad32";
	}

	template<>
	std::string GetTypeName<math::Radian<f64>>(void)
	{
		return "rad64";
	}


	template<>
	std::string GetTypeName<math::Degree<f32>>(void)
	{
		return "deg32";
	}

	template<>
	std::string GetTypeName<math::Degree<f64>>(void)
	{
		return "deg64";
	}

// Quaternion

	template<>
	std::string GetTypeName<math::Quatf>(void)
	{
		return "quat32";
	}

	template<>
	std::string GetTypeName<math::Quaternion<f64>>(void)
	{
		return "quat64";
	}

}