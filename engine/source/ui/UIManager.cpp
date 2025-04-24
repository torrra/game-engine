#include "ui/UIManager.h"
#include "Window.h"
#include "window/WindowLib.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>


#include "utility/MemoryCheck.h"
#include "Engine.h"

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

void engine::UIManager::NewFrameNoUI(void)
{
    //ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    NewFrame();
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

void engine::UIManager::EndFrameNoUI(void)
{
    ImGui::PopItemFlag();
    EndFrame();
    //ImGui::PopStyleVar();
}

void engine::UIManager::ShutDown(void)
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
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

    // Init
    ImGui_ImplGlfw_InitForOpenGL(*window, true);
    ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);
    ImGui_ImplOpenGL3_Init("#version 450");
}


