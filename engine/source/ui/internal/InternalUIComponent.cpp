#include "ui/UIComponent.h"

#undef new
#include <imgui/imgui.h>

#include <cstdarg>

void ui::Text(const char* text, ...)
{
    va_list args;
    va_start(args, text);
    ImGui::TextV(text, args);
    va_end(args);
}

bool ui::Button(const char* text, math::Vector2f size)
{
    return ImGui::Button(text, size);
}

bool ui::InputTextBox(const char* hint, std::string& outStr)
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

bool ui::StartMenuBar(void)
{
    return ImGui::BeginMenuBar();
}

bool ui::StartMainMenuBar(void)
{
    return ImGui::BeginMainMenuBar();
}

void ui::EndMenuBar(void)
{
    ImGui::EndMenuBar();
}

void ui::EndMainMenuBar(void)
{
    ImGui::EndMainMenuBar();
}

bool ui::MenuItem(const char* name, const char* shortcut)
{
    return ImGui::MenuItem(name, shortcut);
}

bool ui::StartPopUp(const char* name)
{
    return ImGui::BeginPopupContextItem(name);
}

void ui::EndPopUp(void)
{
    ImGui::EndPopup();
}

void ui::SameLine(void)
{
    ImGui::SameLine(0.0F, -1.0F);
}

void ui::StartDisabledSection(bool disabled)
{
    ImGui::BeginDisabled(disabled);
}

void ui::EndDisabledSection(void)
{
    ImGui::EndDisabled();
}

void ui::SetID(std::string const& id)
{
    ImGui::PushID(id.c_str());
}

void ui::UnsetID(void)
{
    ImGui::PopID();
}

void ui::SetKeyboardFocus(void)
{
    ImGui::SetKeyboardFocusHere();
}
