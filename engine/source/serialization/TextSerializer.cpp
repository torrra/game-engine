#include "serialization/TextSerializer.h"


namespace engine::text
{

    void Serialize(std::ostream& file, const char* name, const std::string& val)
    {
        const char* valName((name) ? name : "unnamed val");

        std::string toWrite;
        toWrite.reserve(val.size());

        for (uint64 index = 0; index < val.size(); ++index)
        {
            if (val[index] == '"')
                toWrite.push_back('\\');

            toWrite.push_back(val[index]);
        }

        file << "string | " << valName << "=\"" << toWrite << "\"";
    }

    void Serialize(std::ostream& file, const char* name, const char* val, uint64 length)
    {
        std::string toWrite(val, length);
        
        Serialize(file, name, toWrite);
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
        while(text != end && *text != '\"')
            ++text;

        if (text != end)
            ++text;

        else
            return text;

    readString:

        while(text != end && *text != '\"')
        {
            val.push_back(*text);
            ++text;
        }

        if (text[-1] == '\\')
        {
            ++text;
            goto readString;
        }

        if (text != end)
            ++text;

        return text;
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

        while (parser::IsNewline(*text) && (text != end))
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

