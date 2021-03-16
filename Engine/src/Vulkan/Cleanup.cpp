#include "Vulkan/VulkanAPI.h"

namespace eng
{
    void VulkanAPI::Cleanup()
    {
        for (auto imageView : m_SwapChainImageViews)
        {
            vkDestroyImageView(m_LogicalDevice, imageView, nullptr);
        }
        vkDestroySwapchainKHR(m_LogicalDevice, m_SwapChain, nullptr);
        vkDestroyDevice(m_LogicalDevice, nullptr);
        if (m_EnableValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
        }
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }
}