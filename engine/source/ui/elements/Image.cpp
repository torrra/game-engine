#include "ui/elements/Image.h"
#include "resource/ResourceManager.h"
#include "resource/texture/Texture.h"

#include <math/Vector2.hpp>

#undef new
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#define ALPHA_BITMASK 0xFF0000

engine::Image::Image(const char* fileName, math::Vector2f const& position)
    : m_fileName(fileName), m_keepAspectRatio(true)
{
    m_data = ResourceManager::GetResource<Texture>(m_fileName.c_str());
    
    // Set transform
    SetPosition(position);
    
    if (m_data)
    {
        math::Vector2i size = m_data->GetSize();
        SetScale(
        {
            static_cast<f32>(size.X()),
            static_cast<f32>(size.Y())
        });
    }
    
    // Default colors
    SetImageTint(1.0f, 1.0f, 1.0f, 1.0f);
    SetBorderColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void engine::Image::Render(void)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    if (window->SkipItems)
        return;
    
    const float borderSize = (m_borderColor & ALPHA_BITMASK) ? 1.0f : 0.0f;
    const math::Vector2f padding(borderSize, borderSize);

    math::Vector2f size;
    if (m_keepAspectRatio)
    {
        size =
        {
            m_transform.m_sizePx.GetX(),
            m_transform.m_sizePx.GetX() * m_data->GetAspectRatio()
        };
    }
    else
        size = m_transform.m_sizePx;
    
    const ImRect box(
        window->Pos + m_transform.m_position, 
        window->Pos + m_transform.m_position + size + padding * 2.0f
    );
    
    ImGui::ItemSize(box);
    if (!ImGui::ItemAdd(box, 0))
        return;

    if (borderSize > 0.0f)
        window->DrawList->AddRect(box.Min, box.Max, m_borderColor, 0.0f, ImDrawFlags_None, borderSize);
        
    window->DrawList->AddImage(
        m_data->GetTexture(), 
        box.Min + padding, box.Max - padding, 
        math::Vector2f::Zero(), 
        math::Vector2f::One(), 
        m_tint
    );
}

void engine::Image::ConstantAspectRatio(bool value)
{
    m_keepAspectRatio = value;
}

void engine::Image::SetBorderColor(f32 red, f32 green, f32 blue, f32 alpha)
{
    m_borderColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::Image::SetImageTint(f32 red, f32 green, f32 blue, f32 alpha)
{
    m_tint = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

bool engine::Image::IsDataValid(void)
{
    return m_data;
}
