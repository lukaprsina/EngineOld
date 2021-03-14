#include "pch.h"

#include "GLFW/GLFW.h"

namespace eng
{
    static void GLFWErrorCallback(int error, const char *description)
    {
        std::cout << "GLFW ERROR [" << error << "]: " << description << '\n';
    }

    GLFW::GLFW(WindowSettings settings)
        : m_WindowSettings(settings)
    {
        glfwInit();

        glfwSetErrorCallback(GLFWErrorCallback);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(m_WindowSettings.Width, m_WindowSettings.Height, m_WindowSettings.Title.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(m_Window, &m_WindowSettings);

        SetCallbacks();
    }

    GLFW::~GLFW()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GLFW::OnUpdate()
    {
        glfwPollEvents();
    }

    void GLFW::GetRequiredInstanceExtensions(const char **&GLFWExtensions, uint32_t &GLFWExtensionCount)
    {
        GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);
    }

    void GLFW::CreateWindowSurface(VkInstance instance, VkSurfaceKHR &surface)
    {
        if (glfwCreateWindowSurface(instance, m_Window, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void GLFW::GetFramebufferSize(int &width, int &height)
    {
        glfwGetFramebufferSize(m_Window, &width, &height);
    }
}