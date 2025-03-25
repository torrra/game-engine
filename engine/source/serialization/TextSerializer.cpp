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

			if (currentChar == -1)
				break;
		}

		if (!file.eof() && currentChar != -1)
			file.seekg(1, std::ios::cur);
	}
}