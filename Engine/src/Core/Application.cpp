#include "pch.h"

#include "Core/Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace eng
{
    Application::Application()
        : m_Running(true), m_GLFW({"Test", 800, 600})
    {
        m_GLFW.SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_GLFW.OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        m_Running = false;
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