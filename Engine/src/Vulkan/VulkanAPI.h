#pragma once

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"

namespace eng
{
    class VulkanAPI
    {
    public:
        VulkanAPI(GLFW &glfw);
        ~VulkanAPI();

    private:
        void createInstance(GLFW &glfw);
        VkInstance instance;
    };
}