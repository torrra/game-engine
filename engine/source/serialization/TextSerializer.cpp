#include "serialization/TextSerializer.h"


namespace engine::text
{
    void Serialize(std::ostream& file, const char* name, const std::string& val)
    {
        const char* valName((name) ? name : "unnamed val");
        file << "@string " << val.size() << " | " << valName << '=' << val;
    }

    void Serialize(std::ostream& file, const char* name, const char* val, uint64 length)
    {
        const char* valName((name) ? name : "unnamed val");
        file << "@string " << length << " | " << valName << '=' << val;
    }

    void MoveCursorToVal(uint64& pos, const std::string& line)
    {
        while (line[pos] != '=' && pos < line.size())
            ++pos;

        // Get to first character of value
        if (pos < line.size())
            ++pos;
    }

    const char* DeserializeString(const char* text, const char* end, std::string& val)
    {
        while (!parser::IsDigit(*text) && text != end)
            ++text;

        uint64 size;
        text = DeserializeInteger<uint64>(text, size);

        val.resize(size);

        MOVE_TEXT_CURSOR(text, end);
        memcpy(val.data(), text, size);

        return text += size;
    }

    const char* LoadFileData(std::ifstream& file, const char*& start, const char*& end)
    {
        file.seekg(0, std::ios::end);
        int64 size = file.tellg();
        file.seekg(0, std::ios::beg);

        char* buf = new char[size + 1];

        file.read(buf, size);

        buf[size] = '\0';
        end = buf + size;
        start = buf;

        return buf;
    }

    void UnloadFileData(const char* data)
    {
        delete[] data;
    }

    const char* GetValuePointer(const char* text, const char* end)
    {
        while (*text != '=' && text != end)
            ++text;

        if (text != end)
            ++text;

        return text;
    }

    const char* GetNewLine(const char* text, const char* end)
    {
        while (!parser::IsNewline(*text) && (text != end))
            ++text;

        if (text != end)
            ++text;

        return text;
    }
}

namespace engine::text::parser
{
    bool IsDigit(char character)
    {
        return (character >= '0') && (character <= '9');
    }

    bool IsNewline(char character)
    {
        return ('\r' == character) || ('\n' == character);
    }

    bool IsWhitespace(char character)
    {
        return (character == ' ') || (character == '\t');
    }

}

