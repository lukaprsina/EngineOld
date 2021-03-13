#include "Vulkan/VulkanAPI.h"

namespace eng
{
    void VulkanAPI::Cleanup()
    {
        vkDestroyDevice(logicalDevice, nullptr);
        if (enableValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        vkDestroyInstance(instance, nullptr);
    }
}