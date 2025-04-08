#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include <string>

namespace ui
{
    enum ETreeNodeFlags
    {
        SELECTED = 1,
        FRAMED = 1 << 1,
        ALLOW_OVERLAP = 1 << 2,
        NO_TREE_PUSH_ON_OPEN = 1 << 3,
        NO_AUTO_OPEN_ON_LOG = 1 << 4,
        DEFAULT_OPEN = 1 << 5,
        OPEN_ON_DOUBLE_CLICK = 1 << 6,
        OPEN_WITH_ARROW = 1 << 7,
        NO_ICON = 1 << 8,
        BULLET_ICON = 1 << 9,
        FRAME_PADDING = 1 << 10,
        SPPAN_AVAIL_WIDTH = 1 << 11,
        SPAN_FULL_WIDTH = 1 << 12,
        SPAN_LABEL_WIDTH = 1 << 13,
        SPAN_ALL_COLUMNS = 1 << 14,
        LABEL_SPAN_ALL_COLUMNS = 1 << 15,
        NAV_LEFT_JUMP_BACK_HERE = 1 << 17,
        COLLAPSING_HEADER = FRAMED | NO_TREE_PUSH_ON_OPEN | NO_AUTO_OPEN_ON_LOG
    };

    ENGINE_API bool TreeNode(std::string const& name, int32 flags);
    ENGINE_API void EndTreeNode(void);
}