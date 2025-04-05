#pragma once

#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

struct ImDrawList;

namespace engine::ui
{
    class DrawList
    {
    public:
        DrawList(void) = delete;
        DrawList(ImDrawList* drawList);
        ~DrawList(void) = default;

        void AddRectFilled(math::Vector2f min, math::Vector2f max, int32 color, f32 rounding = 0.0F);
        void AddRectFilled(math::Vector2f min, math::Vector2f max, math::Vector4f color, f32 rounding = 0.0F);

        operator ImDrawList*(void);
        operator ImDrawList*(void) const;
        DrawList operator=(ImDrawList* drawList);

    private:
        ImDrawList* m_drawList = nullptr;
    };

    DrawList GetForegroundDrawList(void);
    DrawList GetBackgroundDrawList(void);
    DrawList GetWindowDrawList(void);
}