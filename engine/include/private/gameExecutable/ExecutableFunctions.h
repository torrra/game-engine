#pragma once

#include "engine/EngineExport.h"

extern "C"
{
    ENGINE_API void* InitializeEngine(void);
    ENGINE_API void  RunGame(void* data);
    ENGINE_API void  Cleanup(void*& data);
}