#pragma once

#include "Core/Log.h"
#include "PluginManager/PluginManager.h"
#include "GLFW/GLFW.h"
#include "Vulkan/Vulkan.h"
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