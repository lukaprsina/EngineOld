#include "pch.h"

#include "Core/Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace eng
{
    Application::Application()
        : m_Running(true)
    {
        Log::Init();

        /*Plugin<EngineSDK> plugin(fs::current_path().append("libSandbox").c_str());*/
        Plugin<EngineSDK> plugin("C:/Users/User/Desktop/Engine/out/build/x64-Debug/bin/Sandbox.dll");
        plugin.Open();
        std::shared_ptr<EngineSDK> Sandbox = plugin.Get();
        Sandbox->SayHello();
        // Sandbox.reset();
        plugin.Close();

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