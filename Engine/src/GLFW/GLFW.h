#pragma once

#include <GLFW/glfw3.h>
#include "Events/Event.h"

namespace eng
{
    struct WindowSettings
    {
        std::string Title = "Engine";
        unsigned int Width = 1280;
        unsigned int Height = 920;

        bool focused = true;

        std::function<void(Event &)> EventCallback;
    };

    class GLFW
    {
    public:
        GLFW(WindowSettings settings);
        ~GLFW();

        void OnUpdate();

    private:
        GLFWwindow *m_Window;
        WindowSettings m_WindowSettings;

        void SetCallbacks();
    };
}