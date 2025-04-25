#include <iostream>
#include <filesystem>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef void* (__cdecl* VoidPtrRetFunc)(void);
typedef void(__cdecl* NoRetVoidPtrRefFunc)(void*&);

int main(void)
{
    HINSTANCE           libInstance;
    VoidPtrRetFunc      InitEngine;
    NoRetVoidPtrRefFunc RunGame;
    NoRetVoidPtrRefFunc Cleanup;

    libInstance = LoadLibraryA("engine.dll");

    if (libInstance)
    {
        InitEngine = (VoidPtrRetFunc)GetProcAddress(libInstance, "InitializeEngine");
        RunGame = (NoRetVoidPtrRefFunc)GetProcAddress(libInstance, "RunGame");
        Cleanup = (NoRetVoidPtrRefFunc)GetProcAddress(libInstance, "Cleanup");

        if (InitEngine && RunGame && Cleanup)
        {
            void* dataPtr = (InitEngine)();
            (RunGame)(dataPtr);
            (Cleanup)(dataPtr);
        }

        FreeLibrary(libInstance);
    }
    else
        std::cout << GetLastError() << std::endl;

    (void)getchar();
}