#include "Vulkan/VulkanAPI.h"

namespace eng
{
    void VulkanAPI::Cleanup()
    {
        vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
        vkDestroyDevice(logicalDevice, nullptr);
        if (enableValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }
}