#include "ui/UIManager.h"
#include "ui/UIComponent.h"
#include "Window.h"
#include "ui/Canvas.h"
#include "window/WindowLib.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include "ui/UIComponent.h"

#include "utility/MemoryCheck.h"
#include "Engine.h"
#include "ConsoleLog.hpp"

#define ENABLE_UI_DEBUG 0

engine::UIManager::UIManager(wnd::Wnd* window)
{
    InitUI(window);
}

void engine::UIManager::NewFrame(void)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (Engine::HasEditor())
        ImGui::DockSpaceOverViewport();
    
#if ENABLE_UI_DEBUG == 1
    ImGui::ShowMetricsWindow();
#endif
}

void engine::UIManager::EndFrame(void)
{

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backupContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupContext);
    }
}

void engine::UIManager::ShutDown(void)
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

bool engine::UIManager::IsWindowFocused(std::string const& name)
{
    return ::ui::IsWindowSelected(name);
}

void engine::UIManager::CreateCanvas(std::string const& name, math::Vector2f size)
{
    if (m_canvasMap.contains(name))
    {
        PrintLog(WarningPreset(), "Canvas cannot be created as it already exists.");
        return;
    }
    else
    {
        printf("Canvas Lua created from this roblox-esk hell language\n");
        m_canvasMap[name] = new Canvas(size, {0.0f, 0.0f, 0.0f, 0.0f});
    }
}

engine::Canvas* engine::UIManager::GetCanvas(std::string const& name)
{
    if (m_canvasMap.contains(name.c_str()))
        return m_canvasMap[name];
    else
    {
        PrintLog(WarningPreset(), "Canvas does not exist.");
        return nullptr;
    }

}

void engine::UIManager::RenderCanvases(void)
{
    for (auto canvas : m_canvasMap)
    {
        canvas.second->Render();
    }
}

void engine::UIManager::InitUI(wnd::Wnd* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    // Set flags
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    if (Engine::HasEditor())
    {
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    // Set UI theme
    ImGui::StyleColorsDark();
    io.Fonts->AddFontFromFileTTF(".\\fonts\\Roboto-Regular.ttf", 19.0f);

    // Init
    ImGui_ImplGlfw_InitForOpenGL(*window, true);
    ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);
    ImGui_ImplOpenGL3_Init("#version 450");
}


