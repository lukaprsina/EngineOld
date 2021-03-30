#pragma once

#include "Core/Log.h"
#include "Plugin/Plugin.h"

#include "GLFW/GLFW.h"
#include "VulkanAPI/VulkanAPI.h"
#include "EngineSDK.h"

#include "Events/ApplicationEvent.h"

namespace fs = std::filesystem;

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
        bool OnWindowResize(WindowResizeEvent &e);

    private:
        bool m_Running;
    };
}