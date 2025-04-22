#include "ui/UITable.h"
#include "ui/UIComponent.h"

#include <imgui/imgui.h>

ui::Table::Table(const char* name, uint16 columnCount, math::Vector2f const& size)
    : m_name(name), m_columnCount(columnCount), m_size(size), m_flags(0), m_columnIndex(0)
{
}

bool ui::Table::StartTable(void)
{
    return ImGui::BeginTable(m_name.c_str(), m_columnCount, m_flags, m_size);
}

void ui::Table::EndTable(void)
{
    ImGui::EndTable();
    m_columnIndex = 0;
}

void ui::Table::NextRow(void)
{
    ImGui::TableNextRow();
    m_columnIndex = 0;
}

bool ui::Table::NextColumn(f32 width)
{
    width;
    //if (width != 0.0f)
    //    ImGui::SetColumnWidth(m_columnIndex++, width);
    return ImGui::TableNextColumn();
}

void ui::Table::SetFlags(uint32 flags)
{
    m_flags = flags;
}

void ui::Table::SetSize(math::Vector2f const& size)
{
    m_size = size;
}

uint16 ui::Table::GetMaxColumns(void) const noexcept
{
    return m_columnCount;
}