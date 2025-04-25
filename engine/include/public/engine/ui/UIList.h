#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

struct ImGuiListClipper;

namespace ui
{
    class ListClipper
    {
    public:
        ENGINE_API ListClipper(void);
        ENGINE_API ~ListClipper(void);

        ENGINE_API void Begin(int32 itemCount, f32 itemHeight = -1.0f);
        ENGINE_API bool Step(void);
        ENGINE_API void End(void);

        ENGINE_API int32 GetStart(void);
        ENGINE_API int32 GetEnd(void);
    
    private:
        struct ImGuiListClipper* m_clipper;
        bool m_isInit;
    };
}