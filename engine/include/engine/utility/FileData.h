#pragma once

#include <stdio.h>
#include "MemoryCheck.h"

namespace engine
{
	struct FileData
	{
		FileData(void) = default;
		FileData(char* fileContent, int32 size)
			: m_fileContent(fileContent), m_size(size)
		{
		}

		FileData& ReadFile(const char* filePath) noexcept
		{
			FILE* file;
			fopen_s(&file, filePath, "rb");
			
			if (file)
			{
				fseek(file, 0, SEEK_END);
				m_size = (int32) ftell(file);

				m_fileContent = static_cast<char*>(calloc(m_size + 1, sizeof(char)));
				fseek(file, 0, SEEK_SET);
				fread(m_fileContent, sizeof(char), m_size, file);

				fclose(file);
			}

			return *this;
		}

		void Clear(void) noexcept
		{
			if (m_fileContent)
				free(m_fileContent);

			m_fileContent = nullptr;
			m_size = 0;
		}

		char*	m_fileContent;
		int32		m_size;
	};
}