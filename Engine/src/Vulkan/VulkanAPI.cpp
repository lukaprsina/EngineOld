#include "pch.h"
#include "Vulkan/VulkanAPI.h"
#include "GLFW/GLFW.h"

// TODO: log system, assert system

namespace eng
{
    VulkanAPI::VulkanAPI()
        : deviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME}), validationLayers({"VK_LAYER_KHRONOS_validation"})
    {
    }

    VulkanAPI::~VulkanAPI()
    {
    }

    void VulkanAPI::IInit()
    {
        CreateInstance();
        SetupDebugMessenger();
        GLFW::CreateWindowSurface(instance, surface);
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
    }

    void VulkanAPI::IShutdown()
    {
        Cleanup();
    }
}