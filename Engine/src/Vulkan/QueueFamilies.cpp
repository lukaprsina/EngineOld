#include "Vulkan/VulkanAPI.h"

namespace eng
{
    uint32_t VulkanAPI::ScoreQueueFamilies(VkPhysicalDevice device, QueueFamilyIndices &indices)
    {
        // TODO: prefer a family that has all the flags
        uint32_t queueFamilyCount = 0;
        std::vector<VkQueueFamilyProperties> queueFamilies;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (int i = 0; i < queueFamilyCount; i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;
        }

        return 1000;
    }
}