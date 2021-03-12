#include "Vulkan/VulkanAPI.h"

namespace eng
{
    void VulkanAPI::Cleanup()
    {
        if (enableValidationLayers)
        {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        vkDestroyInstance(instance, nullptr);
    }
}