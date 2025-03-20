#pragma once

#include <fstream>
#include <type_traits>
#include <iostream>
#include <utility>

#include "engine/CoreTypes.h"

#include <math/Angle.hpp>
#include <math/Vector2.hpp>
#include <math/Vector3.hpp>
#include <math/Vector4.hpp>
#include <math/Quaternion.hpp>


namespace engine
{
	enum class ESerializedType
	{
		UNKNOWN_TYPE,

		// Fundamental types

		UINT8,
		UINT16,
		UINT32,
		UINT64,

		INT8,
		INT16,
		INT32,
		INT64,

		F32,
		F64,

		STRING,

		// Vector2

		VEC2_UINT8,
		VEC2_UINT16,
		VEC2_UINT32,
		VEC2_UINT64,

		VEC2_INT8,
		VEC2_INT16,
		VEC2_INT32,
		VEC2_INT64,

		VEC2_F32,
		VEC2_F64,

		// Vector3

		VEC3_UINT8,
		VEC3_UINT16,
		VEC3_UINT32,
		VEC3_UINT64,

		VEC3_INT8,
		VEC3_INT16,
		VEC3_INT32,
		VEC3_INT64,

		VEC3_F32,
		VEC3_F64,

		// Vector4

		VEC4_UINT8,
		VEC4_UINT16,
		VEC4_UINT32,
		VEC4_UINT64,

		VEC4_INT8,
		VEC4_INT16,
		VEC4_INT32,
		VEC4_INT64,

		VEC4_F32,
		VEC4_F64,

		// Angle

		DEG_F32,
		DEG_F64,

		RAD_F32,
		RAD_F64
	};
}


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
	void Serialize(std::ofstream& file, const char* name, const TValueType& val);

	void Serialize(std::ofstream& file, const char* name, const std::string& val);
	void Serialize(std::ofstream& file, const char* name, const char* val, uint64 length);

	template <CSerializableType TValueType>
	void Deserialize(std::ifstream& file, TValueType& val);	
}

namespace engine::text::types
{
	template <CSerializableType TValueType>
	std::string GetTypeName(void);

	ESerializedType GetTypeFromName(const std::string& typeName);

	ESerializedType GetUnsignedIntegerTypeFromName(const std::string& typeName);
	ESerializedType GetSignedIntegerTypeFromName(const std::string& typeName);

	ESerializedType GetVectorTypeFromName(const std::string& typeName);

	ESerializedType Get2DVectorTypeFromName(const std::string& typeName);
	ESerializedType GetUnsigned2DVectorTypeFromName(const std::string& typeName);
	ESerializedType GetSigned2DVectorTypeFromName(const std::string& typeName);

}

namespace engine::text
{
	template<CSerializableType TValueType> inline
	void Serialize(std::ofstream& file, const char* name, const TValueType& val)
	{
		using TNameType = std::remove_reference_t<std::remove_const_t<TValueType>>;

		const char* valName((name) ? name : "unnamed val");
		file << "@" <<  types::GetTypeName<TNameType>() << " | " << valName << '=' << val;
	}

	template<CSerializableType TValueType>
	void Deserialize(std::ifstream& file, TValueType& val)
	{
		file >> val;
	}
}


namespace engine::text::types
{
	template<CSerializableType TValueType> inline
	std::string GetTypeName(void)
	{
		return "unrecognized type";
	}

// fundamental types

	template<> inline
	std::string GetTypeName<int32>(void)
	{
		return "int32";
	}

	template<> inline
	std::string GetTypeName<int64>(void)
	{
		return "int64";
	}

	template<> inline
	std::string GetTypeName<int16>(void)
	{
		return "int16";
	}

	template<> inline
	std::string GetTypeName<int8>(void)
	{
		return "int8";
	}

	template<> inline
	std::string GetTypeName<uint32>(void)
	{
		return "uint32";
	}

	template<> inline
	std::string GetTypeName<uint64>(void)
	{
		return "uint64";
	}

	template<> inline
	std::string GetTypeName<uint16>(void)
	{
		return "uint16";
	}

	template<> inline
	std::string GetTypeName<uint8>(void)
	{
		return "uint8";
	}

	template<> inline
	std::string GetTypeName<f32>(void)
	{
		return "f32";
	}

	template<> inline
	std::string GetTypeName<f64>(void)
	{
		return "f64";
	}

// Vector3

	template<> inline
	std::string GetTypeName<math::Vector3f>(void)
	{
		return "vec3f32";
	}

	template<> inline
	std::string GetTypeName<math::Vector3d>(void)
	{
		return "vec3f64";
	}


	template<> inline
	std::string GetTypeName<math::Vector3<uint32>>(void)
	{
		return "vec3ui32";
	}

	template<> inline
	std::string GetTypeName<math::Vector3<uint64>>(void)
	{
		return "vec3ui64";
	}

	template<> inline
	std::string GetTypeName<math::Vector3<uint16>>(void)
	{
		return "vec3ui16";
	}

	template<> inline
	std::string GetTypeName<math::Vector3<uint8>>(void)
	{
		return "vec3ui8";
	}

	template<> inline
	std::string GetTypeName<math::Vector3i>(void)
	{
		return "vec3i32";
	}

	template<> inline
	std::string GetTypeName<math::Vector3<int64>>(void)
	{
		return "vec3i64";
	}

	template<> inline
	std::string GetTypeName<math::Vector3<int16>>(void)
	{
		return "vec3i16";
	}

	template<> inline
	std::string GetTypeName<math::Vector3<int8>>(void)
	{
		return "vec3i8";
	}


// Vector2

	template<> inline
	std::string GetTypeName<math::Vector2f>(void)
	{
		return "vec2f32";
	}

	template<> inline
	std::string GetTypeName<math::Vector2d>(void)
	{
		return "vec2f64";
	}


	template<> inline
	std::string GetTypeName<math::Vector2<uint32>>(void)
	{
		return "vec2ui32";
	}

	template<> inline
	std::string GetTypeName<math::Vector2<uint64>>(void)
	{
		return "vec2ui64";
	}

	template<> inline
	std::string GetTypeName<math::Vector2<uint16>>(void)
	{
		return "vec2ui16";
	}

	template<> inline
	std::string GetTypeName<math::Vector2<uint8>>(void)
	{
		return "vec2ui8";
	}

	template<> inline
	std::string GetTypeName<math::Vector2i>(void)
	{
		return "vec2i32";
	}

	template<> inline
	std::string GetTypeName<math::Vector2<int64>>(void)
	{
		return "vec2i64";
	}

	template<> inline
	std::string GetTypeName<math::Vector2<int16>>(void)
	{
		return "vec2i16";
	}

	template<> inline
	std::string GetTypeName<math::Vector2<int8>>(void)
	{
		return "vec2i8";
	}


// Vector4

	template<> inline
	std::string GetTypeName<math::Vector4f>(void)
	{
		return "vec4f32";
	}

	template<> inline
	std::string GetTypeName<math::Vector4d>(void)
	{
		return "vec4f64";
	}


	template<> inline
	std::string GetTypeName<math::Vector4<uint32>>(void)
	{
		return "vec4ui32";
	}

	template<> inline
	std::string GetTypeName<math::Vector4<uint64>>(void)
	{
		return "vec4ui64";
	}

	template<> inline
	std::string GetTypeName<math::Vector4<uint16>>(void)
	{
		return "vec4ui16";
	}

	template<> inline
	std::string GetTypeName<math::Vector4<uint8>>(void)
	{
		return "vec4ui8";
	}

	template<> inline
	std::string GetTypeName<math::Vector4i>(void)
	{
		return "vec4i32";
	}

	template<> inline
	std::string GetTypeName<math::Vector4<int64>>(void)
	{
		return "vec4i64";
	}

	template<> inline
	std::string GetTypeName<math::Vector4<int16>>(void)
	{
		return "vec4i16";
	}

	template<> inline
	std::string GetTypeName<math::Vector4<int8>>(void)
	{
		return "vec4i8";
	}


// Radian / Degree

	template<> inline
	std::string GetTypeName<math::Radian<f32>>(void)
	{
		return "rad32";
	}

	template<> inline
	std::string GetTypeName<math::Radian<f64>>(void)
	{
		return "rad64";
	}


	template<> inline
	std::string GetTypeName<math::Degree<f32>>(void)
	{
		return "deg32";
	}

	template<> inline
	std::string GetTypeName<math::Degree<f64>>(void)
	{
		return "deg64";
	}

// Quaternion

	template<> inline
	std::string GetTypeName<math::Quatf>(void)
	{
		return "quat32";
	}

	template<> inline
	std::string GetTypeName<math::Quaternion<f64>>(void)
	{
		return "quat64";
	}

}