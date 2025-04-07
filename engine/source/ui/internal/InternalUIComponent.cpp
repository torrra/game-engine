#include "ui/InternalUIComponent.h"

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

bool engine::ui::Button(const char* text, math::Vector2f size)
{
    return ImGui::Button(text, size);
}

bool engine::ui::StartMenuBar(void)
{
    return ImGui::BeginMenuBar();
}

void engine::ui::EndMenuBar(void)
{
    ImGui::EndMenuBar();
}

void engine::ui::StartDisabledSection(bool disabled)
{
    ImGui::BeginDisabled(disabled);
}

void engine::ui::EndDisabledSection(void)
{
    ImGui::EndDisabled();
}
