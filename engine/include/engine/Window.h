#pragma once

#include "EngineExport.h"

#include <math/MathGeneric.hpp>
#include <math/Vector2.hpp>

#include <cstdint>

struct GLFWwindow;

namespace engine
{
	class Window
	{
	public:
		ENGINE_API					Window(const char* title = "Window", uint32_t width = 800, uint32_t height = 600);
		ENGINE_API					~Window(void) = default;

		template<math::CScalarType TValueType = uint32_t>
		TValueType					GetWidth(void) const noexcept;

		template<math::CScalarType TValueType = uint32_t>
		TValueType					GetHeight(void) const noexcept;

		ENGINE_API float			GetAspectRatio(void) const noexcept;
		ENGINE_API GLFWwindow*		GetWindowPtr(void) const noexcept;
		ENGINE_API bool				IsIconified(void) const noexcept;
		ENGINE_API bool				IsFullScreen(void) const noexcept;
		ENGINE_API bool				ShouldWindowClose(void);
		ENGINE_API void				MakeCurrentContext(void);

		ENGINE_API void				ClearWindow(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);
		ENGINE_API void				UpdateBuffers(void);
		ENGINE_API void				Shutdown(void);

		ENGINE_API static GLFWwindow* GetCurrentContext(void);
	private:
		int							Init(void);
		inline int					InitGLFW(void);
		inline int					InitGlad(void);
		inline int					CreateWindow(void);

		void						SetSize(uint32_t width, uint32_t height);
		void						SetAspectRatio(void);

		// Window callbacks
		static void					SizeCallback(GLFWwindow* window, int x, int y);

		math::Vector2<uint32_t>		m_size;
		GLFWwindow*					m_windowPtr;
		const char*					m_title;
		float						m_aspectRatio;
	};

	// Template function definitions
	template<math::CScalarType TValueType>
	TValueType Window::GetWidth(void) const noexcept
	{
		return static_cast<TValueType>(m_size.GetX());
	}

	template<math::CScalarType TValueType>
	TValueType Window::GetHeight(void) const noexcept
	{
		return static_cast<TValueType>(m_size.GetY());
	}

}