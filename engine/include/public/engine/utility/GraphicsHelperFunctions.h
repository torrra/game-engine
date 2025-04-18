#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include <math/Vector2.hpp>

namespace engine
{
    ENGINE_API void SendCmdsToGPU(void);
    ENGINE_API void SetPixelStorageMode(void);
    ENGINE_API void GetPixelData(math::Vector2i position, math::Vector2i size, void* data);
}