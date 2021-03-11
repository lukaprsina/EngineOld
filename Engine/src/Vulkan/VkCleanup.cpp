#include "Vulkan/VulkanAPI.h"

namespace eng
{
    void VulkanAPI::Cleanup()
    {
        vkDestroyInstance(instance, nullptr);
    }
}