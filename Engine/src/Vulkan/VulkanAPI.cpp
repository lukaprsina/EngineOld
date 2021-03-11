#include "pch.h"
#include "Vulkan/VulkanAPI.h"

// TODO: log system, assert system

namespace eng
{
    VulkanAPI::VulkanAPI(GLFW &glfw)
        : validationLayers({"VK_LAYER_KHRONOS_validation"})
    {
        CreateInstance(glfw);
    }

    VulkanAPI::~VulkanAPI()
    {
        Cleanup();
    }
}