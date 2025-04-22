#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>

namespace engine
{
    class FrameBuffer
    {
    public:
        ENGINE_API FrameBuffer(void);
        ENGINE_API ~FrameBuffer(void);

        ENGINE_API void Init(int32 width, int32 height);
        ENGINE_API void RescaleFBO(int32 width, int32 height);
        ENGINE_API uint64 GetFrameTexture(void) const noexcept;
        ENGINE_API void Bind(void) const;
        ENGINE_API void Unbind(void) const;

    private:
        void DeleteFrameBuffer(void);

        math::Vector2i m_size;
        uint32 m_colorTex;
        uint32 m_fbo;
        uint32 m_rbo;
    };
}