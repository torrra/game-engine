#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include <math/Vector2.hpp>

namespace engine
{
    ENGINE_API void SendCmdsToGPU(void);
    ENGINE_API void SetPixelStorageMode(void);
    ENGINE_API void GetPixelData(math::Vector2i position, math::Vector2i size, void* data);
    ENGINE_API void Draw(uint32 vao, uint32 indexCount);
    ENGINE_API void EnableDepthTest(void);
    ENGINE_API void DisableDepthTest(void);
}