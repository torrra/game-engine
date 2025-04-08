#include "ui/UITree.h"

#undef new
#include <imgui/imgui.h>

bool ui::TreeNode(std::string const& name, int32 flags)
{
    return ImGui::TreeNodeEx(name.c_str(), flags);
}

void ui::EndTreeNode(void)
{
    ImGui::TreePop();
}