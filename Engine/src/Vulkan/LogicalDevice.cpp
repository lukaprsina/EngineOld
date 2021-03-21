#include "Vulkan/LogicalDevice.h"
#include "Vulkan/PhysicalDevice.h"

namespace eng
{
    LogicalDevice::LogicalDevice()
    {
        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = Vulkan::Get().physicalDevice->m_VkQueueCreateInfos.size();
        createInfo.pQueueCreateInfos = Vulkan::Get().physicalDevice->m_VkQueueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = Vulkan::Get().DeviceExtensions.size();
        createInfo.ppEnabledExtensionNames = Vulkan::Get().DeviceExtensions.data();

        if (Vulkan::AreValidationLayersEnabled())
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(Vulkan::Get().ValidationLayers.size());
            createInfo.ppEnabledLayerNames = Vulkan::Get().ValidationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(Vulkan::Get().physicalDevice->GPUProperties.device, &createInfo, nullptr, &m_VkLogicalDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(m_VkLogicalDevice, Vulkan::Get().physicalDevice->GPUProperties.graphicsFamily.value(), 0, &m_VkGraphicsQueue);
        vkGetDeviceQueue(m_VkLogicalDevice, Vulkan::Get().physicalDevice->GPUProperties.presentFamily.value(), 0, &m_VkPresentQueue);
    }

    LogicalDevice::~LogicalDevice()
    {
        vkDestroyDevice(m_VkLogicalDevice, nullptr);
    }
}