#include "utility/Platform.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

namespace engine
{
    void OpenFile(const wchar_t* filePath, const wchar_t* workingDir)
    {
        ShellExecuteW(NULL, NULL, filePath, NULL, workingDir, SW_SHOW);
    }

    void OpenFile(const char* filePath, const char* workingDir)
    {
        ShellExecuteA(NULL, NULL, filePath, NULL, workingDir, SW_SHOW);
    }
}