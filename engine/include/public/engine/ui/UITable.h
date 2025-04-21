#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include <math/Vector2.hpp>
#include <string>

namespace ui
{
    enum ETableFlags
    {
        // Feature flags
        TABLE_RESIZABLE                         = 1 << 0,   // Can manually resize column width
        TABLE_REORDERABLE                       = 1 << 1,   // Swap column position
        TABLE_HIDEABLE                          = 1 << 2,   // Hide columns
        TABLE_SORTABLE                          = 1 << 3,   // Order column by clicking on column header
        TABLE_NO_METADATA                       = 1 << 4,   // Do not save metadata about column order, width and sorting
        TABLE_CONTEXT_MENU_IN_BODY              = 1 << 5,   // Show table options via right click pop up menu 

        // Decoration Flags
        TABLE_ROW_BG                            = 1 << 6,
        TABLE_INNER_HORIZONTAL_BORDERS          = 1 << 7,
        TABLE_OUTER_HORIZONTAL_BORDERS          = 1 << 8,
        TABLE_INNER_VERTICAL_BORDERS            = 1 << 9,
        TABLE_OUTER_VERTICAL_BORDERS            = 1 << 10,
        TABLE_HORIZONTAL_BORDERS                = TABLE_INNER_HORIZONTAL_BORDERS | TABLE_OUTER_HORIZONTAL_BORDERS,
        TABLE_VERTICAL_BORDERS                  = TABLE_INNER_VERTICAL_BORDERS | TABLE_OUTER_VERTICAL_BORDERS,
        TABLE_ALL_BORDERS                       = TABLE_HORIZONTAL_BORDERS | TABLE_VERTICAL_BORDERS,
        
        // Scrolling
        TABLE_SCROLL_X                          = 1 << 25,
        TABLE_SCROLL_Y                          = 1 << 26
    };

    class Table
    {
    public:
        ENGINE_API Table(const char* name, uint16 columnCount, math::Vector2f const& size = {0.0f, 0.0f});
        ENGINE_API ~Table(void) = default;

        ENGINE_API bool StartTable(void);
        ENGINE_API void EndTable(void);
        ENGINE_API void NextRow(void);
        ENGINE_API bool NextColumn(f32 width = 0.0f);

        ENGINE_API void SetFlags(uint32 flags);
        ENGINE_API void SetSize(math::Vector2f const& size);
    private:
        std::string m_name;
        math::Vector2f m_size;
        uint32 m_flags;
        uint16 m_columnCount;
        uint16 m_columnIndex;

    };
}