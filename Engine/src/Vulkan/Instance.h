#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class Instance
    {
    public:
        Instance();
        ~Instance();

        VkInstance m_VkInstance;
        VkSurfaceKHR m_VkSurface;

    private:
        std::vector<const char *> GetInstanceExtensions();
    };
}