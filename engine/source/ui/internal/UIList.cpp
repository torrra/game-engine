#include "ui/UIList.h"
#include <imgui/imgui.h>
#include "utility/MemoryCheck.h"


ui::ListClipper::ListClipper(void)
    : m_isInit(false), m_clipper(nullptr)
{
}

ui::ListClipper::~ListClipper(void)
{
    delete m_clipper;

    m_clipper = nullptr;
    m_isInit = false;
}

void ui::ListClipper::Begin(int32 itemCount, f32 itemHeight)
{
    if (!m_isInit)
    {
        m_clipper = new ImGuiListClipper;
        m_isInit = true;
    }

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

int32 ui::ListClipper::GetStart(void)
{
    return m_clipper->DisplayStart;
}

int32 ui::ListClipper::GetEnd(void)
{
    return m_clipper->DisplayEnd;
}