#include "serialization/TextSerializer.h"


namespace engine::text
{
	void Serialize(std::ofstream& file, const char* name, const std::string& val)
	{
		const char* valName((name) ? name : "unnamed val");
		file << "@" << "string " << val.size() << " | " << valName << '=' << val;
	}

	void Serialize(std::ofstream& file, const char* name, const char* val, uint64 length)
	{
		const char* valName((name) ? name : "unnamed val");
		file << "@" << "string " << length << " | " << valName << '=' << val;
	}

	void Deserialize(std::ifstream& file, std::string& val)
	{
		char currentChar = static_cast<char>(file.peek());

		while (currentChar <= '0' || currentChar >= '9')
		{
			if (currentChar == '\r' || currentChar == '\n')
				file.seekg(2, std::ios::cur);
			else
				file.seekg(1, std::ios::cur);

			currentChar = static_cast<char>(file.peek());
		}

		uint64 size;

		file >> size;
		val.resize(size);

		MoveCursorToVal(file);
		file.read(val.data(), size);
	}

	void MoveCursorToVal(uint64& pos, const std::string& line)
	{
		while (line[pos] != '=' && pos < line.size())
			++pos;

		// Get to first character of value
		if (pos < line.size())
			++pos;
	}

	void MoveCursorToVal(std::ifstream& file)
	{
		char currentChar = static_cast<char>(file.peek());

		while (currentChar != '=' && !file.eof())
		{
			if (currentChar == '\r' || currentChar == '\n')
				file.seekg(2, std::ios::cur);
			else
				file.seekg(1, std::ios::cur);

			currentChar = static_cast<char>(file.peek());
		}

		if (!file.eof())
			file.seekg(1, std::ios::cur);
	}
}

namespace engine::text::types
{
	ESerializedType GetTypeFromName(const std::string& typeName)
	{
		switch (typeName[1])
		{
			// unsigned integer type
		case 'u': return GetUnsignedIntegerTypeFromName(typeName);

			// signed integer type
		case 'i': return GetSignedIntegerTypeFromName(typeName);

			// vector type
		case 'v': return GetVectorTypeFromName(typeName);

			// string
		case 's': return ESerializedType::STRING;

			// quaternion
		case 'q':

			if (typeName[5] == '3')return ESerializedType::QUAT_F32;

			else return ESerializedType::QUAT_F64;

			// radian
		case 'r':

			if (typeName[4] == '3') return ESerializedType::RAD_F32;

			else return ESerializedType::RAD_F64;

			// degree
		case 'd':

			if (typeName[4] == '3') return ESerializedType::DEG_F32;

			else return ESerializedType::DEG_F64;

		default: return ESerializedType::UNKNOWN_TYPE;
		}
	}


	ESerializedType GetUnsignedIntegerTypeFromName(const std::string& typeName)
	{
		switch (typeName[5])
		{
		// uint8
		case '8':
			return ESerializedType::UINT8;

		// uint16
		case '1':
			return ESerializedType::UINT16;

		// uint32
		case '3':
			return ESerializedType::UINT32;

		// uint64
		case '6':
			return ESerializedType::UINT64;

		// not a type listed above
		default:
			return ESerializedType::UNKNOWN_TYPE;
		}
	}

	ESerializedType GetSignedIntegerTypeFromName(const std::string& typeName)
	{
		switch (typeName[4])
		{
		// int8
		case '8':
			return ESerializedType::INT8;

		// int16
		case '1':
			return ESerializedType::INT16;

		// int32
		case '3':
			return ESerializedType::INT32;

		// int64
		case '6':
			return ESerializedType::INT64;

		// not a type listed above
		default:
			return ESerializedType::UNKNOWN_TYPE;
		}
	}

	ESerializedType GetVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[4])
		{
		case '2':
			return Get2DVectorTypeFromName(typeName);

		case '3':
			return Get3DVectorTypeFromName(typeName);

		case '4':
			return Get4DVectorTypeFromName(typeName);

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}
	}

	ESerializedType Get2DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[5])
		{
		case 'u':
			return GetUnsigned2DVectorTypeFromName(typeName);

		case 'i':
			return GetSigned2DVectorTypeFromName(typeName);

		case 'f':

			if (typeName[6] == '3')
				return ESerializedType::VEC2_F32;
			else
				return ESerializedType::VEC2_F64;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}
	}

	ESerializedType GetUnsigned2DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[7])
		{
		case '8':
			return ESerializedType::VEC2_UINT8;

		case '1':
			return ESerializedType::VEC2_UINT16;

		case '3':
			return ESerializedType::VEC2_UINT32;

		case '6':
			return ESerializedType::VEC2_UINT64;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}

	}

	ESerializedType GetSigned2DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[6])
		{
		case '8':
			return ESerializedType::VEC2_INT8;

		case '1':
			return ESerializedType::VEC2_INT16;

		case '3':
			return ESerializedType::VEC2_INT32;

		case '6':
			return ESerializedType::VEC2_INT64;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}

	}




	ESerializedType Get3DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[5])
		{
		case 'u':
			return GetUnsigned3DVectorTypeFromName(typeName);

		case 'i':
			return GetSigned3DVectorTypeFromName(typeName);

		case 'f':

			if (typeName[6] == '3')
				return ESerializedType::VEC3_F32;
			else if (typeName[6] == '6')
				return ESerializedType::VEC3_F64;

			else return ESerializedType::UNKNOWN_TYPE;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}
	}

	ESerializedType GetUnsigned3DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[7])
		{
		case '8':
			return ESerializedType::VEC3_UINT8;

		case '1':
			return ESerializedType::VEC3_UINT16;

		case '3':
			return ESerializedType::VEC3_UINT32;

		case '6':
			return ESerializedType::VEC3_UINT64;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}

	}

	ESerializedType GetSigned3DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[6])
		{
		case '8':
			return ESerializedType::VEC3_INT8;

		case '1':
			return ESerializedType::VEC3_INT16;

		case '3':
			return ESerializedType::VEC3_INT32;

		case '6':
			return ESerializedType::VEC3_INT64;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}

	}





	ESerializedType Get4DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[5])
		{
		case 'u':
			return GetUnsigned4DVectorTypeFromName(typeName);

		case 'i':
			return GetSigned4DVectorTypeFromName(typeName);

		case 'f':

			if (typeName[6] == '3')
				return ESerializedType::VEC4_F32;
			else if (typeName[6] == '6')
				return ESerializedType::VEC4_F64;

			else return ESerializedType::UNKNOWN_TYPE;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}
	}

	ESerializedType GetUnsigned4DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[7])
		{
		case '8':
			return ESerializedType::VEC4_UINT8;

		case '1':
			return ESerializedType::VEC4_UINT16;

		case '3':
			return ESerializedType::VEC4_UINT32;

		case '6':
			return ESerializedType::VEC4_UINT64;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}

	}

	ESerializedType GetSigned4DVectorTypeFromName(const std::string& typeName)
	{
		switch (typeName[6])
		{
		case '8':
			return ESerializedType::VEC4_INT8;

		case '1':
			return ESerializedType::VEC4_INT16;

		case '3':
			return ESerializedType::VEC4_INT32;

		case '6':
			return ESerializedType::VEC4_INT64;

		default:
			return ESerializedType::UNKNOWN_TYPE;
		}

	}
}
