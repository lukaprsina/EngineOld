#pragma once

#include "Core/Log.h"
#include "GLFW/GLFW.h"
#include "Vulkan/VulkanAPI.h"

#include "Events/ApplicationEvent.h"

namespace eng
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        void OnEvent(Event &e);
        bool OnWindowClose(WindowCloseEvent &e);

    private:
        bool m_Running;
    };
}