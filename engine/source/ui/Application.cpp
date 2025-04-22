#include "core/SceneGraph.h"
#include "game/GameScene.h"
#include "thread/ThreadManager.h"
#include "ui/Application.h"

#include <glad/glad.h>

#define DEFAULT_NAME "Editor"

namespace engine
{
    Application::~Application(void)
    {
        
    }


    int16 Application::Startup(const char* projectName)
    {
        if (m_started)
            return SUCCESS;

        if (Window::StartUp())
            return ERROR;

        m_window = new Window((projectName) ? projectName : DEFAULT_NAME);
        // TODO: change dimensions however keep for debugging
        if (m_window->CreateWindow(1920, 1080))
            return ERROR;

        m_uiManager = UIManager(m_window->GetPtr());
        m_started = true;
        SetBgColor({ 0.f, 0.5f, 1.f, 1.f });
        return SUCCESS;
    }

    void Application::BeginFrame(void)
    {
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_window->ClearWindow(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);
        m_uiManager.NewFrame();
    }

    void Application::EndFrame(void)
    {
        m_uiManager.UpdateUI();
        m_window->Update();
    }

    void Application::SetBgColor(const math::Vector4f& color)
    {
        m_bgColor = color;
       // glClearColor(color.GetX(), color.GetY(), color.GetZ(), color.GetW());
    }

    Window* Application::GetWindow(void) const noexcept
    {
        return m_window;
    }

    void Application::SetCurrentScene(GameScene*)
    {
    }

    void Application::Render(SceneGraph* scene)
    {
        ThreadManager::RenderScene(scene);
    }

    void Application::Shutdown(void)
    {
        Window::ShutDown();
        m_uiManager.ShutDown();

        if (m_window)
            delete m_window;
    }
}