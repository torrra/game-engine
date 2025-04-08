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

bool engine::ui::InputTextBox(const char* hint, std::string& outStr)
{
    constexpr size_t size = 128;
    char buffer[size] = "";

    if (ImGui::InputTextWithHint("##Rename", hint, buffer, size, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        outStr = buffer;
        return true;
    }

    return false;
}

bool engine::ui::StartMenuBar(void)
{
    return ImGui::BeginMenuBar();
}

void engine::ui::EndMenuBar(void)
{
    ImGui::EndMenuBar();
}

bool engine::ui::MenuItem(const char* name, const char* shortcut)
{
    return ImGui::MenuItem(name, shortcut);
}

bool engine::ui::StartPopUp(const char* name)
{
    return ImGui::BeginPopupContextItem(name);
}

void engine::ui::EndPopUp(void)
{
    ImGui::EndPopup();
}

void engine::ui::SameLine(void)
{
    ImGui::SameLine(0.0F, -1.0F);
}

void engine::ui::StartDisabledSection(bool disabled)
{
    ImGui::BeginDisabled(disabled);
}

void engine::ui::EndDisabledSection(void)
{
    ImGui::EndDisabled();
}

void engine::ui::SetID(std::string const& id)
{
    ImGui::PushID(id.c_str());
}

void engine::ui::UnsetID(void)
{
    ImGui::PopID();
}

void engine::ui::SetKeyboardFocus(void)
{
    ImGui::SetKeyboardFocusHere();
}
