#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "Events/Event.h"

namespace eng
{
    struct WindowSettings
    {
        std::string Title = "Engine";
        unsigned int Width = 1280;
        unsigned int Height = 920;

        bool focused = true;

        std::function<void(Event &)> EventCallback = nullptr;
    };

    class GLFW
    {
    public:
        GLFW(WindowSettings settings);
        ~GLFW();

        void OnUpdate();
        inline void SetEventCallback(const std::function<void(Event &)> &callback) { m_WindowSettings.EventCallback = callback; }

        void GetRequiredInstanceExtensions(const char **&GLFWExtensions, uint32_t &GLFWExtensionCount);

        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR &surface);

    private:
        GLFWwindow *m_Window;
        WindowSettings m_WindowSettings;

        void SetCallbacks();
    };
}