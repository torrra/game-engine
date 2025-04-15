#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

struct ImGuiPayload;

namespace engine
{
    namespace ui
    {
        struct UIWindowRect;

    }
}

namespace ui
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
        ENGINE_API Payload(void) = delete;
        ENGINE_API Payload(const ImGuiPayload* payload);
        ENGINE_API ~Payload(void) = default;

        //template <typename TDataType>
        ENGINE_API void* GetData(void) const;
        ENGINE_API bool IsPreview(void) const;
        ENGINE_API bool IsDelivery(void) const;

        ENGINE_API operator const ImGuiPayload*(void);
        ENGINE_API operator const ImGuiPayload*(void) const;
        ENGINE_API Payload& operator=(const ImGuiPayload* payload);    
    private:
        const ImGuiPayload* m_payload = nullptr;
    };

    // Source functions
    ENGINE_API bool StartDragDropSource(::int32 flags);
    ENGINE_API void EndDragDropSource(void);

    // Target functions
    ENGINE_API bool StartDragDropTarget(void);
    ENGINE_API bool StartDragDropTargetCustom(struct engine::ui::UIWindowRect rect, const char* payloadID);
    ENGINE_API void EndDragDropTarget(void);

    // Payload functions
    ENGINE_API void CreatePayload(const char* payloadID, const void* data, uint64 size);
    ENGINE_API Payload AcceptPayload(const char* payloadID, ::int32 flags);
}

//template<typename TDataType>
//inline TDataType engine::ui::Payload::GetData(void) const
//{
//    return reinterpret_cast<TDataType>(m_payload->Data);
//}
