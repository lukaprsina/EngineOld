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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(m_WindowSettings.Width, m_WindowSettings.Height, m_WindowSettings.Title.c_str(), nullptr, nullptr);

        //glfwSetWindowUserPointer(m_Window, &m_WindowSettings);

        //SetCallbacks();
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
}