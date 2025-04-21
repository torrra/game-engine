#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

struct ImDrawList;

namespace ui
{
    class DrawList
    {
    public:
        ENGINE_API DrawList(void) = delete;
        DrawList(ImDrawList* drawList);
        ENGINE_API ~DrawList(void) = default;

        ENGINE_API void AddRectFilled(math::Vector2f const& min, math::Vector2f const& max, int32 color, f32 rounding = 0.0F);
        ENGINE_API void AddRectFilled(math::Vector2f const& min, math::Vector2f const& max, math::Vector4f const& color, f32 rounding = 0.0F);

        operator ImDrawList*(void);
        operator ImDrawList*(void) const;
        DrawList operator=(ImDrawList* drawList);

    private:
        ImDrawList* m_drawList = nullptr;
    };

    ENGINE_API DrawList GetForegroundDrawList(void);
    ENGINE_API DrawList GetBackgroundDrawList(void);
    ENGINE_API DrawList GetWindowDrawList(void);
}