#pragma once

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"

namespace eng
{
    class VulkanAPI
    {
    public:
        VulkanAPI();
        ~VulkanAPI();

    private:
        void createInstance();
        VkInstance instance;
    };
}