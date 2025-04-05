#include "ui/InternalUIStyle.h"

#undef new
#include <imgui/imgui.h>

uint32 ui::GetColor(EColor colorID, f32 alpha)
{
    return ImGui::GetColorU32(colorID, alpha);
}