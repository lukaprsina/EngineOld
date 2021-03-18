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

        unsigned int Framerate = 0;
        bool Focused = true;

        std::function<void(Event &)> EventCallback = nullptr;
    };

    class GLFW
    {
    public:
        GLFW();
        ~GLFW();

        GLFW(const GLFW &) = delete;
        GLFW &operator=(GLFW const &) = delete;

        static GLFW &Get()
        {
            static GLFW instance;
            return instance;
        }

        static void Init(WindowSettings settings) { return Get().IInit(settings); }
        static void Shutdown() { return Get().IShutdown(); }

        static void OnUpdate() { return Get().IOnUpdate(); }

        static void SetEventCallback(const std::function<void(Event &)> &callback) { return Get().ISetEventCallback(callback); }

        static void GetRequiredInstanceExtensions(const char **&GLFWExtensions, uint32_t &GLFWExtensionCount) { return Get().IGetRequiredInstanceExtensions(GLFWExtensions, GLFWExtensionCount); }

        static void CreateWindowSurface(VkInstance &instance, VkSurfaceKHR &surface) { return Get().ICreateWindowSurface(instance, surface); }

        static void GetFramebufferSize(int &width, int &height) { return Get().IGetFramebufferSize(width, height); }

    private:
        GLFWwindow *m_Window;
        WindowSettings m_WindowSettings;

        void IInit(WindowSettings settings);
        void IShutdown();

        void IOnUpdate();

        inline void ISetEventCallback(const std::function<void(Event &)> &callback) { m_WindowSettings.EventCallback = callback; }

        void IGetRequiredInstanceExtensions(const char **&GLFWExtensions, uint32_t &GLFWExtensionCount);

        void ICreateWindowSurface(VkInstance &instance, VkSurfaceKHR &surface);

        void IGetFramebufferSize(int &width, int &height);

        void SetCallbacks();
    };
}