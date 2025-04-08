#include "resource/font/Font.h"

#include <imgui/imgui.h>

#include <iostream>
#include "utility/MemoryCheck.h"

#define DEFAULT_FONT_SIZE 24.0f

engine::Font::Font(void)
	: m_font(nullptr)
{
}

engine::Font::~Font(void)
{
	m_font->ClearOutputData();
}

bool engine::Font::LoadResource(const char* fileName)
{	
	ImGuiIO& io = ImGui::GetIO();

	// TODO: scale via DPI?
    m_font = io.Fonts->AddFontFromFileTTF(fileName, DEFAULT_FONT_SIZE);

	if(!m_font)
        return false;
    
    // Build to font atlas
    io.Fonts->Build();
    return true;
}

ImFont* engine::Font::GetFont(void) const noexcept
{
	return m_font;
}

f32 engine::Font::GetFontSize(void) const noexcept
{
	return m_font->FontSize;
}
