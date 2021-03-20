#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class PhysicalDevice
    {
    public:
        PhysicalDevice();
        ~PhysicalDevice();

        float QueuePriority = 1.0f;
        DeviceInfo GPUProperties;
        std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;

        DeviceInfo GetQueueFamilies(const VkPhysicalDevice &device);
        bool CheckDeviceExtensionSupport(const VkPhysicalDevice &device);
        uint32_t ScorePhysicalDevice(const DeviceInfo &indices);
    };
}