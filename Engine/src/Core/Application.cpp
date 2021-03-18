#include "pch.h"

#include "Core/Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace eng
{
    Application::Application()
        : m_Running(true)
    {
        Log::Init();

        WindowSettings settings;

        GLFW::Init(settings);
        GLFW::SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        Vulkan::Init();
    }

    Application::~Application()
    {
        Vulkan::Shutdown();
        GLFW::Shutdown();
    }

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
    }

    void Application::Run()
    {
        while (m_Running)
        {
            GLFW::OnUpdate();
            Vulkan::OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        e.Handled = true;
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent &e)
    {
        Vulkan::OnWindowResize(e);
        return true;
    }
}

int main()
{
    eng::Application *app = new eng::Application;
    app->Run();
    delete app;

    return 0;
}