#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#include <math/MathGeneric.hpp>
#include <math/Vector2.hpp>

#include <cstdint>
#include <string>

struct GLFWwindow;

namespace engine
{
	class Window
	{
	public:
		ENGINE_API					Window(const char* title = "Window", uint32 width = 800, uint32 height = 600);
		ENGINE_API					~Window(void) = default;

		template<math::CScalarType TValueType = uint32>
		TValueType					GetWidth(void) const noexcept;

		template<math::CScalarType TValueType = uint32>
		TValueType					GetHeight(void) const noexcept;

		ENGINE_API f32				GetAspectRatio(void) const noexcept;
		ENGINE_API GLFWwindow*		GetWindowPtr(void) const noexcept;
		ENGINE_API bool				IsIconified(void) const noexcept;
		ENGINE_API bool				IsFullScreen(void) const noexcept;
		ENGINE_API bool				ShouldWindowClose(void);
		ENGINE_API void				MakeCurrentContext(void);

		ENGINE_API void				ClearWindow(f32 red = 0.0f, f32 green = 0.0f, f32 blue = 0.0f, f32 alpha = 1.0f);
		ENGINE_API void				UpdateBuffers(void);
		ENGINE_API void				Shutdown(void);

		ENGINE_API static GLFWwindow* GetCurrentContext(void);
	private:
		int32						Init(void);
		inline int32				InitGLFW(void);
		inline int32				InitGlad(void);
		inline int32				CreateWindow(void);

		void						SetSize(uint32 width, uint32 height);
		void						SetAspectRatio(void);

		// Window callbacks
		static void					SizeCallback(GLFWwindow* window, int32 x, int32 y);

		math::Vector2<uint32>		m_size;
		GLFWwindow*					m_windowPtr;
		std::string					m_title;
		f32							m_aspectRatio;
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