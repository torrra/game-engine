#include "ui/UIComponent.h"
#include "CoreTypes.h"

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

void ui::Image(uint64 textureID, math::Vector2f const& size)
{
    ImGui::Image(textureID, size, {0.0f, 1.0f}, {1.0f, 0.0f});
}

bool ui::Button(const char* text, math::Vector2f size)
{
    return ImGui::Button(text, size);
}

bool ui::InputBox(const char* id, const char* hint, std::string& outStr)
{
    constexpr size_t size = 128;
    char buffer[size] = "";

    if (ImGui::InputTextWithHint(id, hint, buffer, size, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        outStr = buffer;
        return true;
    }

    return false;
}

bool ui::InputBox(const char* id, f32* value, f32 increment, bool returnOnComplete)
{
    bool result = ImGui::DragFloat(id, value, increment);

    if (returnOnComplete)
        result = ImGui::IsItemDeactivatedAfterEdit();

    return result;
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

void ui::ItemWidth(f32 widthPx)
{
    ImGui::SetNextItemWidth(widthPx);
}

void ui::VerticalSpacing(void)
{
    ImGui::Spacing();
}

math::Vector2f ui::GetPos(void)
{
    return ImGui::GetCursorPos();
}

math::Vector2f ui::GetAvailSpace(void)
{
    return ImGui::GetContentRegionAvail();
}

void ui::StartSection(const char* name)
{
    constexpr int32 flags = 
        ImGuiChildFlags_Borders | 
        ImGuiChildFlags_AlwaysUseWindowPadding | 
        ImGuiChildFlags_AutoResizeY;

    ImGui::BeginChild(name, ImVec2(0, 0), flags);
}

void ui::EndSection(void)
{
    ImGui::EndChild();
}

void ui::StartDisabledSection(bool disabled)
{
    ImGui::BeginDisabled(disabled);
}

void ui::EndDisabledSection(void)
{
    ImGui::EndDisabled();
}

bool ui::CollapseSection(const char* name)
{
    return ImGui::CollapsingHeader(name, ImGuiTreeNodeFlags_None);
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

bool ui::IsItemSelected(void)
{
    return ImGui::IsItemClicked(ImGuiMouseButton_Left);
}

bool ui::IsItemHovered(void)
{
    return ImGui::IsItemHovered();
}
