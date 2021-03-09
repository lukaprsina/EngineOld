#pragma once

#include "GLFW/GLFW.h"
#include "Vulkan/VulkanAPI.h"

namespace eng
{
    class Application
    {
    public:
        Application();
        ~Application();

        Application(const Application *&) = delete;
        Application &operator=(const Application *&) = delete;

        void Run();
        bool OnWindowClose();

    private:
        bool m_Running;
        GLFW m_GLFW;
        VulkanAPI m_VkAPI;
    };
}