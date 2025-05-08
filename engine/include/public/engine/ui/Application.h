#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/Window.h"

#include "engine/ui/UIManager.h"
#include "engine/ui/UIWindow.h"

#include <math/Vector4.hpp>

#include <filesystem>

namespace engine
{
    class Application
    {
    public:

        ENGINE_API Application(void) = default;

        Application(const Application&) = delete;
        Application(Application&&) = delete;

        ENGINE_API virtual ~Application(void) = default;

        ENGINE_API virtual int16 Startup(const char* projectName);
        ENGINE_API virtual void  BeginFrame(void);
        ENGINE_API virtual void  Render(class SceneGraph* scene);
        ENGINE_API virtual void  EndFrame(void);
        ENGINE_API virtual void  Shutdown(void);
        ENGINE_API virtual void  SetCurrentScene(class GameScene*);
        
        ENGINE_API void SetBgColor(const math::Vector4f& color);
        ENGINE_API Window* GetWindow(void) const noexcept;

    private:

        math::Vector4f          m_bgColor;
        Window*                 m_window = nullptr;
        bool                    m_started = false;

    };
}