#include "Vulkan/VulkanAPI.h"

namespace eng
{
    DeviceIndices VulkanAPI::GetQueueFamilies(VkPhysicalDevice device)
    {
        // TODO: prefer a family that has all the flags
        DeviceIndices indices;
        uint32_t queueFamilyCount = 0;

        std::vector<VkQueueFamilyProperties> queueFamilies;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        indices.device = device;

        for (int i = 0; i < queueFamilyCount; i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;
        }

        return indices;
    }
}