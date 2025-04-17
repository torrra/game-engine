#pragma once

#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>

struct ImRect;

namespace engine::ui
{
    struct UIWindowRect
    {
        UIWindowRect(void) = default;
        UIWindowRect(math::Vector2f const& min, math::Vector2f const& max);

        operator ImRect(void);
        operator ImRect(void) const;

        math::Vector2f m_min{0.0f, 0.0f};
        math::Vector2f m_max{0.0f, 0.0f};
    };

    void StartWindowRenderer(const char* title, int32 flags);
    void EndWindowRenderer(void);
    
    void SetWindowPos(math::Vector2f const& position, int32 flag);
    void SetWindowSize(math::Vector2f const& size, int32 flag);
    const UIWindowRect GetCurrentWindowRect(void);
}