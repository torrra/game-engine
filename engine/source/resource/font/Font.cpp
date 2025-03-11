#include "resource/font/Font.h"

#include <imgui/imgui.h>

#include <iostream>
#include "utility/MemoryCheck.h"

#define RELATIVE_FONT_PATH "..\\fonts\\"

engine::Font::Font(void)
	: m_font(nullptr)
{
}

engine::Font::~Font(void)
{
	m_font->ClearOutputData();
}

void engine::Font::LoadResource(const char* fileName)
{
	// Concatenate file path
	char filePath[256] = RELATIVE_FONT_PATH;
	strcat_s(filePath, fileName);
	
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.RasterizerDensity = 1.0f;
	m_font = io.Fonts->AddFontFromFileTTF(filePath, 36.0f, &config);
	io.Fonts->Build();
}

ENGINE_API ImFont* engine::Font::GetFont(void) const noexcept
{
	if (!this)
		return nullptr;

	return m_font;
}

ENGINE_API f32 engine::Font::GetFontSize(void) const noexcept
{
	return m_font->FontSize;
}
