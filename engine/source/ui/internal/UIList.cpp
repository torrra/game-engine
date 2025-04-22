#include "ui/UIList.h"
#include <imgui/imgui.h>
#include "utility/MemoryCheck.h"


ui::ListClipper::ListClipper(void)
{
    m_clipper = new ImGuiListClipper;
}

ui::ListClipper::~ListClipper(void)
{
    delete m_clipper;
}

void ui::ListClipper::Begin(int32 itemCount, f32 itemHeight)
{
    m_clipper->Begin(itemCount, itemHeight);
}

bool ui::ListClipper::Step(void)
{
    return m_clipper->Step();
}

void ui::ListClipper::End(void)
{
    m_clipper->End();
}
