#include "pch.h"

#include "Core/Application.h"

namespace eng
{
    Application::Application()
        : m_Running(true), m_GLFW({"Test", 800, 600})
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_GLFW.OnUpdate();
        }
    }

    bool Application::OnWindowClose()
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