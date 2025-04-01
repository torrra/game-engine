#pragma once

#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>

namespace engine
{
    class FrameBuffer
    {
    public:
         FrameBuffer(void);
        ~FrameBuffer(void);

        void Init(int32 width, int32 height);
        void RescaleFBO(int32 width, int32 height);
        uint64 GetFrameTexture(void) const noexcept;
        void Bind(void) const;
        void Unbind(void) const;

    private:
        void DeleteFrameBuffer(void);

        math::Vector2i m_size;
        uint32 m_colorTex;
        uint32 m_fbo;
        uint32 m_rbo;
    };
}