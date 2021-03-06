#include "pch.h"

#include "GLFW/GLFW.h"

namespace eng
{
    static void GLFWErrorCallback(int error, const char *description)
    {
        std::cout << "GLFW ERROR [" << error << "]: " << description << '\n';
    }

    GLFW::GLFW()
    {
    }

    GLFW::~GLFW()
    {
    }

    void GLFW::IInit(WindowSettings settings)
    {
        m_WindowSettings = settings;

        glfwInit();
        glfwSetErrorCallback(GLFWErrorCallback);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        if (settings.Fullscreen)
        {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            m_Window = glfwCreateWindow(mode->width, mode->height, m_WindowSettings.Title.c_str(), monitor, nullptr);
        }
        else
        {
            m_Window = glfwCreateWindow(m_WindowSettings.Width, m_WindowSettings.Height, m_WindowSettings.Title.c_str(), nullptr, nullptr);
        }
        glfwSetWindowUserPointer(m_Window, &m_WindowSettings);

        SetCallbacks();
    }

    void GLFW::IShutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GLFW::IOnUpdate()
    {
        glfwPollEvents();
    }

    void GLFW::IGetRequiredInstanceExtensions(const char **&GLFWExtensions, uint32_t &GLFWExtensionCount)
    {
        GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);
    }

    void GLFW::ICreateWindowSurface(VkInstance &m_Instance, VkSurfaceKHR &surface)
    {
        if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void GLFW::IGetFramebufferSize(int &width, int &height)
    {
        glfwGetFramebufferSize(m_Window, &width, &height);
    }
}