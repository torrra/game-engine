#include "ui/UIComponent.h"
#include "CoreTypes.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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

bool ui::Button(const char* text, math::Vector2f const& size)
{
    return ImGui::Button(text, size);
}

bool ui::DropDown(const char* id, int32& selectedValue, std::vector<const char*> const& options)
{
    int32 arraySize = static_cast<int32>(options.size());

    return ImGui::Combo(id, &selectedValue, options.data(), arraySize);
}

bool ui::DropDown(const char* id, int32& selectedValue,const char** options, int32 size)
{
    return ImGui::Combo(id, &selectedValue, options, size);
}

bool ui::InputBox(const char* id, const char* hint, std::string& outStr, bool enterReturnTrue)
{
    constexpr size_t size = 128;
    char buffer[size] = "";

    if (ImGui::InputTextWithHint(id, hint, buffer, size, (enterReturnTrue) ? ImGuiInputTextFlags_EnterReturnsTrue : 0))
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

bool ui::Selectable(const char* text, bool* selected, math::Vector2f const& size)
{
    return ImGui::Selectable(text, selected, ImGuiMultiSelectFlags_SingleSelect, size);
}

bool ui::Checkbox(const char* text, bool* value)
{
    return ImGui::Checkbox(text, value);
}

bool ui::ColorEdit(const char* id, f32* values)
{
    constexpr ImGuiColorEditFlags flags = ImGuiColorEditFlags_PickerHueBar |
                                          ImGuiColorEditFlags_DisplayRGB;

    return ImGui::ColorEdit3(id, values, flags);;
}

bool ui::StartMenuBar(void)
{
    return ImGui::BeginMenuBar();
}

bool ui::StartMainMenuBar(void)
{
    return ImGui::BeginMainMenuBar();
}

bool ui::StartMenu(const char* name)
{
    return ImGui::BeginMenu(name);
}

void ui::EndMenuBar(void)
{
    ImGui::EndMenuBar();
}

void ui::EndMainMenuBar(void)
{
    ImGui::EndMainMenuBar();
}

void ui::EndMenu(void)
{
    ImGui::EndMenu();
}

bool ui::MenuItem(const char* name, const char* shortcut)
{
    return ImGui::MenuItem(name, shortcut);
}

bool ui::StartModal(const char* name, bool& shouldClose)
{
    int32 flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    return ImGui::BeginPopupModal(name, &shouldClose, flags);;
}

bool ui::StartPopUp(const char* name)
{
    return ImGui::BeginPopupContextItem(name);
}

void ui::EndPopUp(void)
{
    ImGui::EndPopup();
}

void ui::OpenModal(const char* name)
{
    ImGui::OpenPopup(name);
}

void ui::SameLine(f32 xOffset)
{
    ImGui::SameLine(xOffset, -1.0F);
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

math::Vector2f ui::GetScreenPos(void)
{
    return ImGui::GetCursorScreenPos();
}

math::Vector2f ui::GetAvailSpace(void)
{
    return ImGui::GetContentRegionAvail();
}

math::Vector2i ui::GetViewportPos(std::string const& windowName)
{
    ImGuiWindow* window = ImGui::FindWindowByName(windowName.c_str());
    
    if (window)
        return math::Vector2i(
                static_cast<int32>(window->ViewportPos.x),
                static_cast<int32>(window->ViewportPos.y)
        );

    return math::Vector2i::Zero();
}

math::Vector2i ui::GetWindowPos(std::string const& windowName)
{
    ImGuiWindow* window = ImGui::FindWindowByName(windowName.c_str());

    if (window)
        return math::Vector2i(
                static_cast<int32>(window->Pos.x),
                static_cast<int32>(window->Pos.y)
        );

    return math::Vector2i::Zero();
}

math::Vector2f ui::GetWindowPos(void)
{
    return ImGui::GetWindowPos();
}

math::Vector2i ui::GetOuterRectMinPos(std::string const& windowName)
{
    ImGuiWindow* window = ImGui::FindWindowByName(windowName.c_str());

    if (window)
    {
        ImVec2 position = window->OuterRectClipped.Min;
        return math::Vector2i(
            static_cast<int32>(position[0]),
            static_cast<int32>(position[1])
        );
    }

    return math::Vector2i::Zero();
}

math::Vector2i ui::GetInnerRectMinPos(std::string const& windowName)
{
    ImGuiWindow* window = ImGui::FindWindowByName(windowName.c_str());

    if (window)
    {
        const ImVec2& position = window->ContentRegionRect.Min;
        return math::Vector2i(
            static_cast<int32>(position[0]),
            static_cast<int32>(position[1])
        );
    }

    return math::Vector2i::Zero();
}

void ui::SetAlignment(math::Vector2f const& gridIndex)
{
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, gridIndex);
}

void ui::SetScreenPosition(math::Vector2f const& position)
{
    ImGui::SetCursorScreenPos(position);
}

void ui::UnsetAlignment(void)
{
    return ImGui::PopStyleVar();
}

bool ui::TreeNode(const char* sectionName)
{
    return ImGui::TreeNode(sectionName);
}

void ui::TreePop(void)
{
    ImGui::TreePop();
}

bool ui::StartSection(const char* name)
{
    constexpr int32 flags = 
        ImGuiChildFlags_Borders | 
        ImGuiChildFlags_AlwaysUseWindowPadding | 
        ImGuiChildFlags_AutoResizeY;

    return ImGui::BeginChild(name, ImVec2(0, 0), flags);
}

ENGINE_API bool ui::StartSection(const char* name, math::Vector2f const& size)
{
    constexpr int32 flags =
        ImGuiChildFlags_Borders |
        ImGuiChildFlags_AlwaysUseWindowPadding |
        ImGuiChildFlags_AutoResizeY;

    return ImGui::BeginChild(name, size, flags);
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

bool ui::CollapseSection(const char* name, bool& closeButton)
{
    constexpr int32 flags = 
        ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_AllowOverlap | 
        ImGuiTreeNodeFlags_ClipLabelForTrailingButton | ImGuiTreeNodeFlags_DefaultOpen;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    if (window->SkipItems)
        return false;

    uint32 id = window->GetID(name);

    bool is_open = ImGui::TreeNodeBehavior(id, flags, name);
    {
        ImGuiContext& context = *GImGui;
        ImGuiLastItemData last_item_backup = context.LastItemData;
        float button_size = context.FontSize;

        math::Vector2f buttonSize(
            ImMax(context.LastItemData.Rect.Min.x, context.LastItemData.Rect.Max.x - context.Style.FramePadding.x - button_size),
            context.LastItemData.Rect.Min.y + context.Style.FramePadding.y
        );

        uint32  close_button_id = ImGui::GetIDWithSeed("#CLOSE", NULL, id);
        closeButton = (ImGui::CloseButton(close_button_id, buttonSize));
        context.LastItemData = last_item_backup;
    }

    return is_open;
}

void ui::SetID(std::string const& id)
{
    ImGui::PushID(id.c_str());
}

void ui::SetID(int32 id)
{
    ImGui::PushID(id);
}

void ui::UnsetID(void)
{
    ImGui::PopID();
}

math::Vector2i ui::GetWindowCursorPos(std::string const& wndName)
{
    math::Vector2f mousePos = ImGui::GetMousePos();
    math::Vector2i wndPos = GetInnerRectMinPos(wndName);
    
    return 
    {
        (int32) mousePos.GetX() - wndPos.GetX(),
        (int32) mousePos.GetY() - wndPos.GetY()
    };
}

math::Vector2f ui::GetWindowContentSize(std::string const& wndName)
{
    if (ImGuiWindow* window = ImGui::FindWindowByName(wndName.c_str()))
        return window->ContentSize;
    
    return math::Vector2f::Zero();
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

bool ui::IsWindowSelected(std::string const& name)
{
    ImGuiContext* context = ImGui::GetCurrentContext();

    return ImGui::FindWindowByName(name.c_str()) == context->NavWindow;
}

bool ui::IsWindowDocked(std::string const& windowName)
{
    ImGuiWindow* window = ImGui::FindWindowByName(windowName.c_str());

    if (window)
        return window->DockIsActive;

    return false;
}

bool ui::IsRectVisible(math::Vector2f const& size)
{
    return ImGui::IsRectVisible(size);
}

bool ui::IsRectVisible(math::Vector2f const& min, math::Vector2f const& max)
{
    return ImGui::IsRectVisible(min, max);
}
