#pragma once

#include "engine/EngineExport.h"

struct ImGuiIO;
struct GLFWwindow;

namespace engine
{
	class UIManager
	{
	public:
		UIManager(void) = delete;
		ENGINE_API UIManager(GLFWwindow* window);
		~UIManager(void) = default;

		ENGINE_API void NewFrame(void);
		ENGINE_API void UpdateUI(void);
		ENGINE_API void ShutDown(void);
	private:
		void InitUI(GLFWwindow* window);
	};
}