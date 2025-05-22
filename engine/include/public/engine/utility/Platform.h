#pragma once

#include "engine/EngineExport.h"

namespace engine
{
    ENGINE_API void OpenFile(const wchar_t* filePath, const wchar_t* workingDir = nullptr);
    ENGINE_API void OpenFile(const char* filePath, const char* workingDir = nullptr);
}