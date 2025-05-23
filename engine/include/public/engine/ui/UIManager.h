#pragma once

#include "engine/EngineExport.h"
#include <math/Vector2.hpp>
#include <string>
#include <unordered_map>

struct ImGuiIO;
namespace engine::wnd
{
	class Wnd;
}

namespace engine
{
	class UIManager
	{
	public:
		// Initialize UI, must be called after creating GLFW window
		ENGINE_API		UIManager(wnd::Wnd* window);
		ENGINE_API		UIManager(void) = default;
		ENGINE_API		~UIManager(void) = default;

        // Update function, call at start of update loop after reset window color
        ENGINE_API void	NewFrame(void);

        // Update function, call at the end of the update loop before updating window buffers
        ENGINE_API void	EndFrame(void);

		// Clear memory call at end of program
		ENGINE_API void	ShutDown(void);

        ENGINE_API bool IsWindowFocused(std::string const& name);

        ENGINE_API void CreateCanvas(std::string const& name, math::Vector2f size);
        ENGINE_API class Canvas* GetCanvas(std::string const& name);
        ENGINE_API void RenderCanvases(void);
	
	private:
		void			InitUI(wnd::Wnd* window);

        std::unordered_map<std::string, class Canvas*> m_canvasMap;
	};
}