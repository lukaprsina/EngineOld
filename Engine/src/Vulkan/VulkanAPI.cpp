#include "pch.h"
#include "Vulkan/VulkanAPI.h"
#include "GLFW/GLFW.h"

// TODO: log system, assert system

namespace eng
{
    VulkanAPI::VulkanAPI()
        : m_DeviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME}), m_ValidationLayers({"VK_LAYER_KHRONOS_validation"})
    {
    }

    VulkanAPI::~VulkanAPI()
    {
    }

    void VulkanAPI::IInit()
    {
        CreateInstance();
        SetupDebugMessenger();
        GLFW::CreateWindowSurface(m_Instance, m_Surface);
        PickPhysicalDevice();
        CreateLogicalDevice();

        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
        CreateFramebuffers();
        CreateCommandPool();
        CreateCommandBuffers();
        CreateSemaphores();
    }

    void VulkanAPI::IOnUpdate()
    {
        DrawFrame();
    }

    void VulkanAPI::IShutdown()
    {
        Cleanup();
    }
}