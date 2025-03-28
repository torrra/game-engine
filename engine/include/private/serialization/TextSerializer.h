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

#define MOVE_TEXT_CURSOR(text, end) text = text::GetValuePointer(text, end);\
if (text == end) return text

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
	void Serialize(std::ostream& file, const char* name, const TValueType& val);

	void Serialize(std::ostream& file, const char* name, const std::string& val);
	void Serialize(std::ostream& file, const char* name, const char* val, uint64 length);

	void MoveCursorToVal(uint64& pos, const std::string& line);

	template <math::CIntegralType TValueType>
	const char* DeserializeInteger(const char* text, TValueType& val);
	
	template<math::CFloatingType TValueType>
	const char* DeserializeReal(const char* text, TValueType& val);

	template <uint8 TDimensionsUint8, math::CScalarType TValueType>
	const char* DeserializeVector(const char* text, math::Vector<TDimensionsUint8, TValueType>& val);

	template<math::CFloatingType TValueType>
	const char* DeserializeQuaternion(const char* text, math::Quaternion<TValueType>& val);

	const char* DeserializeString(const char* text, const char* end, std::string & val);

	const char* LoadFileData(std::ifstream& file, const char*& start, const char*& end);
	void UnloadFileData(const char* data);

	// Equivalent to MoveCursorToVal, but with another name
	// to avoid confusion
	const char* GetValuePointer(const char* text, const char* end);
	const char* GetNewLine(const char* text, const char* end);
}


namespace engine::text::parser
{
	template <math::CScalarType TValueType>
	TValueType CharToDigit(char character);

	bool IsDigit(char character);
	bool IsNewline(char character);
	bool IsWhitespace(char character);

	template <math::CScalarType TValueType>
	const char* GetValSign(const char* text, TValueType& sign);

	template <math::CFloatingType TValueType>
	const char* ParseIntegralPart(const char* ptr, TValueType& result);

	template <math::CFloatingType TValueType>
	const char* ParseDecimalPart(const char* ptr, TValueType& result);

	template <math::CFloatingType TValueType>
	const char* ParseExponent(const char* ptr, TValueType& exponent);
}


namespace engine::text::types
{
	template <CSerializableType TValueType>
	constexpr const char* GetTypeName(void);
}

namespace engine::text
{
	template<CSerializableType TValueType> inline
	void Serialize(std::ostream& file, const char* name, const TValueType& val)
	{
		using TNameType = std::remove_reference_t<std::remove_const_t<TValueType>>;

		const char* valName((name) ? name : "unnamed val");
		file << "@" <<  types::GetTypeName<TNameType>() << " | " << valName << '=' << val;
	}

	template<math::CIntegralType TValueType>
	const char* DeserializeInteger(const char* text, TValueType& val)
	{
		val = 0;
		TValueType sign;
		text = parser::GetValSign(text, sign);
		
		while (parser::IsDigit(*text))
		{
			val = val * static_cast<TValueType>(10) + parser::CharToDigit<TValueType>(*text);
			++text;
		}

		val *= sign;
		return text;
	}


	template<math::CFloatingType TValueType>
	const char* DeserializeReal(const char* text, TValueType& val)
	{
		TValueType   sign = static_cast<TValueType>(1);
		text = parser::GetValSign(text, sign);

		TValueType   parsed = static_cast<TValueType>(0);
		TValueType   decimal = static_cast<TValueType>(0);
		text = parser::ParseIntegralPart(text, parsed);

		if ('.' == *text)
			++text;

		text = parser::ParseDecimalPart(text, decimal);
		parsed += decimal;

		if ('e' == *text || 'E' == *text)
		{
			TValueType exponent = static_cast<TValueType>(0);
			text = parser::ParseExponent(text, exponent);
			parsed *= exponent;
		}
		parsed *= sign;
		val = parsed;
		return text;
	}


	template<uint8 TDimensionsUint8, math::CScalarType TValueType>
	const char* DeserializeVector(const char* text, math::Vector<TDimensionsUint8, TValueType>& val)
	{
		++text;

		if constexpr (std::is_integral<TValueType>::value)
		{
			for (uint8 index = 0; index < TDimensionsUint8; ++index)
			{
				text = DeserializeInteger<TValueType>(text, val[static_cast<int32>(index)]);
				++text;
			}
		}
		else
		{
			for (uint8 index = 0; index < TDimensionsUint8; ++index)
			{
				text = DeserializeReal<TValueType>(text, val[static_cast<int32>(index)]);
				++text;
			}

		}
		return text;
	}


	template<math::CFloatingType TValueType>
	const char* DeserializeQuaternion(const char* text, math::Quaternion<TValueType>& val)
	{
		++text;
		text = DeserializeReal<TValueType>(text, val.W());
		++text;

		text = DeserializeReal<TValueType>(text, val.X());
		++text;

		text = DeserializeReal<TValueType>(text, val.Y());
		++text;

		text = DeserializeReal<TValueType>(text, val.Z());
		++text;

		return text;
	}

}


namespace engine::text::types
{
	template<CSerializableType TValueType> inline
	constexpr const char* GetTypeName(void)
	{
		return "unrecognized type";
	}

// fundamental types

	template<> inline
	constexpr const char* GetTypeName<bool>(void)
	{
		return "bool";
	}

	template<> inline
	constexpr const char* GetTypeName<int32>(void)
	{
		return "int32";
	}

	template<> inline
	constexpr const char* GetTypeName<int64>(void)
	{
		return "int64";
	}

	template<> inline
	constexpr const char* GetTypeName<int16>(void)
	{
		return "int16";
	}

	template<> inline
	constexpr const char* GetTypeName<int8>(void)
	{
		return "int8";
	}

	template<> inline
	constexpr const char* GetTypeName<uint32>(void)
	{
		return "uint32";
	}

	template<> inline
	constexpr const char* GetTypeName<uint64>(void)
	{
		return "uint64";
	}

	template<> inline
	constexpr const char* GetTypeName<uint16>(void)
	{
		return "uint16";
	}

	template<> inline
	constexpr const char* GetTypeName<uint8>(void)
	{
		return "uint8";
	}

	template<> inline
	constexpr const char* GetTypeName<f32>(void)
	{
		return "f32";
	}

	template<> inline
	constexpr const char* GetTypeName<f64>(void)
	{
		return "f64";
	}

// Vector3

	template<> inline
	constexpr const char* GetTypeName<math::Vector3f>(void)
	{
		return "vec3f32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector3d>(void)
	{
		return "vec3f64";
	}


	template<> inline
	constexpr const char* GetTypeName<math::Vector3<uint32>>(void)
	{
		return "vec3ui32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector3<uint64>>(void)
	{
		return "vec3ui64";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector3<uint16>>(void)
	{
		return "vec3ui16";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector3<uint8>>(void)
	{
		return "vec3ui8";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector3i>(void)
	{
		return "vec3i32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector3<int64>>(void)
	{
		return "vec3i64";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector3<int16>>(void)
	{
		return "vec3i16";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector3<int8>>(void)
	{
		return "vec3i8";
	}


// Vector2

	template<> inline
	constexpr const char* GetTypeName<math::Vector2f>(void)
	{
		return "vec2f32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector2d>(void)
	{
		return "vec2f64";
	}


	template<> inline
	constexpr const char* GetTypeName<math::Vector2<uint32>>(void)
	{
		return "vec2ui32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector2<uint64>>(void)
	{
		return "vec2ui64";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector2<uint16>>(void)
	{
		return "vec2ui16";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector2<uint8>>(void)
	{
		return "vec2ui8";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector2i>(void)
	{
		return "vec2i32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector2<int64>>(void)
	{
		return "vec2i64";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector2<int16>>(void)
	{
		return "vec2i16";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector2<int8>>(void)
	{
		return "vec2i8";
	}


// Vector4

	template<> inline
	constexpr const char* GetTypeName<math::Vector4f>(void)
	{
		return "vec4f32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector4d>(void)
	{
		return "vec4f64";
	}


	template<> inline
	constexpr const char* GetTypeName<math::Vector4<uint32>>(void)
	{
		return "vec4ui32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector4<uint64>>(void)
	{
		return "vec4ui64";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector4<uint16>>(void)
	{
		return "vec4ui16";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector4<uint8>>(void)
	{
		return "vec4ui8";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector4i>(void)
	{
		return "vec4i32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector4<int64>>(void)
	{
		return "vec4i64";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector4<int16>>(void)
	{
		return "vec4i16";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Vector4<int8>>(void)
	{
		return "vec4i8";
	}


// Radian / Degree

	template<> inline
	constexpr const char* GetTypeName<math::Radian<f32>>(void)
	{
		return "rad32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Radian<f64>>(void)
	{
		return "rad64";
	}


	template<> inline
	constexpr const char* GetTypeName<math::Degree<f32>>(void)
	{
		return "deg32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Degree<f64>>(void)
	{
		return "deg64";
	}

// Quaternion

	template<> inline
	constexpr const char* GetTypeName<math::Quatf>(void)
	{
		return "quat32";
	}

	template<> inline
	constexpr const char* GetTypeName<math::Quaternion<f64>>(void)
	{
		return "quat64";
	}

}


namespace engine::text::parser
{
	template<math::CScalarType TValueType>
	TValueType CharToDigit(char character)
	{
		return static_cast<TValueType>(character - '0');
	}

	template<math::CScalarType TValueType>
	const char* GetValSign(const char* text, TValueType& sign)
	{
		sign = static_cast<TValueType>(1);

		switch (*text)
		{
		case '-':
			++text;
			sign = static_cast<TValueType>(-1);
			break;

		case '+':
			++text;
			break;

		default: break;
		}

		return text;
	}


	template<math::CFloatingType TValueType>
	const char* ParseIntegralPart(const char* text, TValueType& result)
	{
		while (IsDigit(*text))
		{
			result = result * static_cast<TValueType>(10) + CharToDigit<TValueType>(*text);
			++text;
		}
		return text;
	}

	template<math::CFloatingType TValueType>
	const char* ParseDecimalPart(const char* ptr, TValueType& result)
	{
		TValueType   divisor = static_cast<TValueType>(1);

		while (IsDigit(*ptr))
		{
			result = result * static_cast<TValueType>(10) + CharToDigit<TValueType>(*ptr);
			divisor *= static_cast<TValueType>(10);
			++ptr;
		}

		result = result / divisor;
		return ptr;
	}

	template<math::CFloatingType TValueType>
	const char* ParseExponent(const char* ptr, TValueType& exponent)
	{
		static constexpr TValueType negativePowers[] =
		{
			(TValueType) 1e0, (TValueType)1e-1,(TValueType)1e-2,(TValueType)1e-3,(TValueType)1e-4,
			(TValueType)1e-5,(TValueType)1e-6,(TValueType)1e-7,(TValueType)1e-8,(TValueType)1e-9,
			(TValueType)1e-10,(TValueType)1e-11,(TValueType)1e-12,(TValueType)1e-13,
			(TValueType)1e-14, (TValueType)1e-15, (TValueType)1e-16, (TValueType)1e-17,(TValueType)1e-18,(TValueType)1e-19
		};
		static constexpr TValueType positivePowers[] =
		{
			(TValueType)1e0, (TValueType)1e1,(TValueType)1e2, (TValueType)1e3, (TValueType)1e4,
			(TValueType)1e5,(TValueType)1e6,(TValueType)1e7, (TValueType)1e8,(TValueType)1e9,
			(TValueType)1e10, (TValueType)1e11,(TValueType)1e12,(TValueType)1e13,
			(TValueType)1e14, (TValueType)1e15,(TValueType)1e16, (TValueType)1e17,(TValueType)1e18, (TValueType)1e19
		};

		const TValueType*	powers = positivePowers;
		int32               index = 0;

		++ptr;
		switch (*ptr)
		{
		case '-':
			++ptr;
			powers = negativePowers;
			break;

		case '+':
			++ptr;
			break;

		default: break;
		}

		while (IsDigit(*ptr))
		{
			index = index * 10 + CharToDigit<int32>(*ptr);
			++ptr;
		}
		exponent = (index >= 20) ? static_cast<TValueType>(0) : powers[index];
		return ptr;
	}
}