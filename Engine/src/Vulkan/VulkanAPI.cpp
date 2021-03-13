#include "pch.h"
#include "Vulkan/VulkanAPI.h"

// TODO: log system, assert system

namespace eng
{
    VulkanAPI::VulkanAPI(GLFW &glfw)
        : deviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME}), validationLayers({"VK_LAYER_KHRONOS_validation"})
    {
        CreateInstance(glfw);
        SetupDebugMessenger();
        glfw.CreateWindowSurface(instance, surface);
        PickPhysicalDevice();
        CreateLogicalDevice();
    }

    VulkanAPI::~VulkanAPI()
    {
        Cleanup();
    }
}