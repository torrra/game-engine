#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>

#include <string>

namespace engine::wnd
{
    class Wnd;
}

namespace engine
{
    /*
       Window class
       This class is used to create a window while not making
       any direct calls to any window dependency. Therefore, the 
       code can be re-used if the window dependency is later changed.
    */
    
    class Window
    {
    public:
        ENGINE_API Window(void) = delete;
        ENGINE_API Window(const char* title);
        ENGINE_API ~Window(void);

        ENGINE_API static int32 StartUp(void);
        ENGINE_API static void ShutDown(void);
        ENGINE_API int32 CreateWindow(void);
        ENGINE_API int32 CreateWindow(int32 width, int32 height);
        ENGINE_API void ClearWindow(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
        ENGINE_API void Update(void);
        ENGINE_API bool ShouldWindowClose(void) const noexcept;

        template<math::CScalarType TValueType = uint32>
        TValueType GetWidth(void) const noexcept;

        template<math::CScalarType TValueType = uint32>
        TValueType GetHeight(void) const noexcept;

        template<math::CScalarType TValueType = uint32>
        math::Vector2<TValueType> GetSize(void) const noexcept;

        ENGINE_API f32 GetAspectRatio(void) const noexcept;
        ENGINE_API engine::wnd::Wnd* GetPtr(void) const noexcept;
        ENGINE_API std::string GetTitle(void) const noexcept;
        ENGINE_API bool IsMinimized(void) const noexcept;
        ENGINE_API bool IsFullScreen(void) const noexcept;
        void SetSize(math::Vector2i const& size);
        void SetAspectRatio(f32 aspectRatio);

    private:
        engine::wnd::Wnd* m_windowPtr;
        std::string m_title;
        math::Vector2i m_size;
        f32 m_aspectRatio;
    };

    template<math::CScalarType TValueType>
    inline TValueType engine::Window::GetWidth(void) const noexcept
    {
        return m_size.GetX();
    }

    template<math::CScalarType TValueType>
    inline TValueType engine::Window::GetHeight(void) const noexcept
    {
        return m_size.GetY();
    }

    template<math::CScalarType TValueType>
    inline math::Vector2<TValueType> Window::GetSize(void) const noexcept
    {
        return
        {
            static_cast<TValueType>(m_size.GetX()),
            static_cast<TValueType>(m_size.GetY())
        };
    }
}