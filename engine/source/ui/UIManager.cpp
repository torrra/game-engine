#include "ui/UIManager.h"
#include "Window.h"
#include "window/WindowLib.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>


#include "utility/MemoryCheck.h"

#define ENABLE_UI_DEBUG 1

engine::UIManager::UIManager(wnd::Wnd* window)
{
	InitUI(window);
}

void engine::UIManager::NewFrame(void)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport();
	
#if ENABLE_UI_DEBUG == 1
	ImGui::ShowMetricsWindow();
#endif
}

void engine::UIManager::UpdateUI(void)
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

void engine::UIManager::InitUI(wnd::Wnd* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	// Set flags
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	
	// Set UI theme
	ImGui::StyleColorsDark();

	// Init
	ImGui_ImplGlfw_InitForOpenGL(*window, true);
	ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);
	ImGui_ImplOpenGL3_Init("#version 450");
}


