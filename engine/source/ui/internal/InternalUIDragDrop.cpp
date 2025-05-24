#include "ui/UIDragDrop.h"
#include "ui/UIWindow.h"
#include "ui/InternalUIWindow.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

bool ui::StartDragDropSource(int32 flags)
{
    return ImGui::BeginDragDropSource(flags);
}

void ui::EndDragDropSource(void)
{
    ImGui::EndDragDropSource();
}

bool ui::StartDragDropTarget(void)
{
    return ImGui::BeginDragDropTarget();
}

bool ui::StartDragDropTargetCustom(engine::ui::UIWindowRect rect, const char* payloadID)
{
    return ImGui::BeginDragDropTargetCustom(rect, ImGui::GetID(payloadID));
}

void ui::EndDragDropTarget(void)
{
    ImGui::EndDragDropTarget();
}

void ui::CreatePayload(const char* payloadID, const void* data, uint64 size)
{
    ImGui::SetDragDropPayload(payloadID, data, size);
}

// Payload
ui::Payload ui::AcceptPayload(const char* payloadID, ::int32 flags)
{
    return ImGui::AcceptDragDropPayload(payloadID, flags);
}

ui::Payload ui::GetPayload(void)
{
    return ImGui::GetDragDropPayload();
}

ui::Payload::Payload(const ImGuiPayload* payload)
{
    m_payload = payload;
}

void* ui::Payload::GetData(void) const
{
    return m_payload->Data;
}

bool ui::Payload::IsPreview(void) const
{
    return m_payload->IsPreview();
}

bool ui::Payload::IsDelivery(void) const
{
    return m_payload->IsDelivery();
}

ui::Payload::operator const ImGuiPayload*(void)
{
    return m_payload;
}

ui::Payload::operator const ImGuiPayload*(void) const
{
    return m_payload;
}

ui::Payload& ui::Payload::operator=(const ImGuiPayload* payload)
{
    m_payload = payload;

    return *this;
}
