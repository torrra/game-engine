#include "ui/InternalUIText.h"

#undef new
#include <imgui/imgui.h>

#include <cstdarg>

void engine::ui::Text(const char* text, ...)
{
    va_list args;
    va_start(args, text);
    ImGui::TextV(text, args);
    va_end(args);
}