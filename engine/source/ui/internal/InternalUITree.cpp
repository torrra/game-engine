#include "ui/InternalUITree.h"

#undef new
#include <imgui/imgui.h>

bool engine::ui::TreeNode(std::string const& name, int32 flags)
{
    return ImGui::TreeNodeEx(name.c_str(), flags);
}

void engine::ui::EndTreeNode(void)
{
    ImGui::TreePop();
}