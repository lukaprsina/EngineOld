#pragma once

#include "VulkanAPI/VulkanAPI.h"

namespace eng
{
    struct DeviceInfo
    {
        VkPhysicalDevice device;
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isOK()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class PhysicalDevice
    {
    public:
        PhysicalDevice();
        ~PhysicalDevice();

        float QueuePriority = 1.0f;
        DeviceInfo GPUProperties;
        std::vector<VkDeviceQueueCreateInfo> m_VkQueueCreateInfos;

        DeviceInfo GetQueueFamilies(const VkPhysicalDevice &device);
        bool CheckDeviceExtensionSupport(const VkPhysicalDevice &device);
        uint32_t ScorePhysicalDevice(const DeviceInfo &indices);
    };

    class LogicalDevice
    {
    public:
        LogicalDevice();
        ~LogicalDevice();

        VkDevice m_VkLogicalDevice;
        VkQueue m_VkGraphicsQueue;
        VkQueue m_VkPresentQueue;
    };
}