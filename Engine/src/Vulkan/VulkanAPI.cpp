#include "pch.h"
#include "Vulkan/VulkanAPI.h"

namespace eng
{
    VulkanAPI::VulkanAPI(GLFW &glfw)
    {
        createInstance(glfw);
    }

    VulkanAPI::~VulkanAPI()
    {
    }
}