#include "core/SceneGraph.h"
#include "game/GameScene.h"
#include "thread/ThreadManager.h"
#include "ui/Application.h"

#include "utility/Colors.h"

#include <glad/glad.h>

#define DEFAULT_NAME "Editor"
#define SUCCESS		0
#define ERROR		1

namespace engine
{
    int16 Application::Startup(const char* projectName)
    {
        if (m_started)
            return SUCCESS;

        if (Window::StartUp())
            return ERROR;

        m_window = new Window((projectName) ? projectName : DEFAULT_NAME);

        // TODO: change dimensions however keep for debugging
        if (m_window->CreateWindow(/*1920, 1080*/))
            return ERROR;

        m_started = true;
        SetBgColor({DARK_GREEN, 1.f });
        return SUCCESS;
    }

    void Application::BeginFrame(void)
    {
        m_window->ClearWindow(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);
    }

    void Application::EndFrame(void)
    {
        m_window->Update();
    }

    void Application::SetBgColor(const math::Vector4f& color)
    {
        m_bgColor = color;
    }

    Window* Application::GetWindow(void) const noexcept
    {
        return m_window;
    }

    void Application::SetCurrentScene(GameScene*)
    {
    }

    void Application::ResetScene(::engine::GameScene&)
    {
    }

    void  Application::LoadNewScene(GameScene& activeScene,
                                    const std::filesystem::path& filePath)
    {
        ThreadManager::SynchronizeGameThread(nullptr);
        activeScene.Stop();
        activeScene.GetGraph()->CleanRigidBodies();


        activeScene.LoadNewScene(false, filePath);
    }

    void Application::Render(SceneGraph* scene)
    {
        ThreadManager::RenderScene(scene);
    }

    void Application::Shutdown(void)
    {
        Window::ShutDown();

        if (m_window)
            delete m_window;
    }
}