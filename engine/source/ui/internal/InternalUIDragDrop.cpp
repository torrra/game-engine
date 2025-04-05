#include "ui/InternalUIDragDrop.h"
#include "ui/InternalUIWindow.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

bool engine::ui::StartDragDropSource(int32 flags)
{
    return ImGui::BeginDragDropSource(flags);
}

void engine::ui::EndDragDropSource(void)
{
    ImGui::EndDragDropSource();
}

bool engine::ui::StartDragDropTarget(void)
{
    return ImGui::BeginDragDropTarget();
}

bool engine::ui::StartDragDropTargetCustom(engine::ui::UIWindowRect rect, const char* payloadID)
{
    return ImGui::BeginDragDropTargetCustom(rect, ImGui::GetID(payloadID));
}

void engine::ui::EndDragDropTarget(void)
{
    ImGui::EndDragDropTarget();
}

void engine::ui::CreatePayload(const char* payloadID, const void* data, uint64 size)
{
    ImGui::SetDragDropPayload(payloadID, data, size);
}

// Payload
engine::ui::Payload engine::ui::AcceptPayload(const char* payloadID, ::int32 flags)
{
    return ImGui::AcceptDragDropPayload(payloadID, flags);
}

engine::ui::Payload::Payload(const ImGuiPayload* payload)
{
    m_payload = payload;
}

void* engine::ui::Payload::GetData(void) const
{
    return m_payload->Data;
}

bool engine::ui::Payload::IsPreview(void) const
{
    return m_payload->IsPreview();
}

bool engine::ui::Payload::IsDelivery(void) const
{
    return m_payload->IsDelivery();
}

engine::ui::Payload::operator const ImGuiPayload*(void)
{
    return m_payload;
}

engine::ui::Payload::operator const ImGuiPayload*(void) const
{
    return m_payload;
}

engine::ui::Payload& engine::ui::Payload::operator=(const ImGuiPayload* payload)
{
    m_payload = payload;

    return *this;
}
