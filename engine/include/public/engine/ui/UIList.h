#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

struct ImGuiListClipper;

namespace ui
{
    class ListClipper
    {
    public:
        ListClipper(void);
        ~ListClipper(void);

        void Begin(int32 itemCount, f32 itemHeight = -1.0f);
        bool Step(void);
        void End(void);
    
    private:
        struct ImGuiListClipper* m_clipper;
    };
}