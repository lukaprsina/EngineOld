#include "pch.h"
#include "Vulkan/Vulkan.h"
#include "GLFW/GLFW.h"

// TODO: log system, assert system

namespace eng
{
    Vulkan::Vulkan()
        : m_DeviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME}), m_ValidationLayers({"VK_LAYER_KHRONOS_validation"})
    {
    }

    Vulkan::~Vulkan()
    {
    }

    void Vulkan::IInit()
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
        CreateSyncObjects();
    }

    void Vulkan::IOnUpdate()
    {
        DrawFrame();
        vkDeviceWaitIdle(m_LogicalDevice);
    }

    void Vulkan::IOnWindowResize(WindowResizeEvent &e)
    {
        m_FramebufferResized = true;
        ENG_CORE_TRACE("{0}", e.ToString());
    }

    void Vulkan::IShutdown()
    {
        Cleanup();
    }
}