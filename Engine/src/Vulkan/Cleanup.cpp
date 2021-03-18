#include "Vulkan/Vulkan.h"

namespace eng
{
    void Vulkan::Cleanup()
    {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(m_LogicalDevice, m_RenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(m_LogicalDevice, m_ImageAvailableSemaphores[i], nullptr);
            vkDestroyFence(m_LogicalDevice, m_InFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);

        CleanupSwapChain();

        vkDestroyDevice(m_LogicalDevice, nullptr);

        if (m_EnableValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }
}