#include "pch.h"

#include "Core/Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace eng
{
    Application::Application()
        : m_Running(true)
    {
        Log::Init();

        lt_dlinit();

        Plugin<EngineSDK> plugin(fs::current_path().append("libSandbox").c_str());
        plugin.DLOpenLib();
        std::shared_ptr<EngineSDK> Sandbox = plugin.DLGetInstance();
        Sandbox->SayHello();

        WindowSettings windowSettings;

        GLFW::Init(windowSettings);
        GLFW::SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        VulkanSettings vulkanSettings;

        Vulkan::Init(vulkanSettings);
    }

    Application::~Application()
    {
        Vulkan::Shutdown();
        GLFW::Shutdown();
        lt_dlexit();
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
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            GLFW::OnUpdate();
            Vulkan::OnUpdate(time);
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
    eng::Application *app = new eng::Application();
    app->Run();
    delete app;

    return 0;
}