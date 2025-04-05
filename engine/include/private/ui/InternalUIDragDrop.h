#pragma once

#include "engine/CoreTypes.h"

struct ImGuiPayload;

namespace engine::ui
{
    enum EDragDropSrcFlags
    {
        // Source flags
        SOURCE_NO_PREVIEW_TOOLTIP = 1,
        NO_DISABLE_HOVER = 2,
        NO_HOLD_TO_OPEN = 1 << 2,
        ALLOW_NULL_ID = 1 << 3,
        SOURCE_EXTERN = 1 << 4,
        PAYLOAD_AUTO_EXPIRE = 1 << 5,
        PAYLOAD_NO_CROSS_CONTEXT = 1 << 6,
        PAYLOAD_NO_CROSS_PROCESS = 1 << 7
    };

    enum EDragDropAcceptFlags
    {
        ACCEPT_BEFORE_DELIVERY = 1 << 10,
        NO_DRAW_OUTLINE_RECT = 1 << 11,
        NO_PREVIEW_TOOLTIP = 1 << 12,
        PEEK_ONLY = ACCEPT_BEFORE_DELIVERY | NO_DRAW_OUTLINE_RECT
    };

    class Payload
    {
    public:
        Payload(void) = delete;
        Payload(const ImGuiPayload* payload);
        ~Payload(void) = default;

        //template <typename TDataType>
        void* GetData(void) const;
        bool IsPreview(void) const;
        bool IsDelivery(void) const;

        operator const ImGuiPayload*(void);
        operator const ImGuiPayload*(void) const;
        Payload& operator=(const ImGuiPayload* payload);    
    private:
        const ImGuiPayload* m_payload = nullptr;
    };

    // Source functions
    bool StartDragDropSource(::int32 flags);
    void EndDragDropSource(void);

    // Target functions
    bool StartDragDropTarget(void);
    bool StartDragDropTargetCustom(struct UIWindowRect rect, const char* payloadID);
    void EndDragDropTarget(void);

    // Payload functions
    void CreatePayload(const char* payloadID, const void* data, uint64 size);
    Payload AcceptPayload(const char* payloadID, ::int32 flags);
}

//template<typename TDataType>
//inline TDataType engine::ui::Payload::GetData(void) const
//{
//    return reinterpret_cast<TDataType>(m_payload->Data);
//}
