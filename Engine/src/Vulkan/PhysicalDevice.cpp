#include "Vulkan/VulkanAPI.h"

// TODO: for loop &
namespace eng
{
    void VulkanAPI::PickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        std::vector<VkPhysicalDevice> devices;

        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        devices.resize(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        std::multimap<int, DeviceInfo> candidates;

        for (const auto &device : devices)
        {
            DeviceInfo indices = GetQueueFamilies(device);
            int score = ScorePhysicalDevice(indices);

            candidates.insert(std::make_pair(score, indices));
        }

        if (candidates.rbegin()->first > 0)
        {
            GPUProperties = candidates.rbegin()->second;
        }
        else
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    uint32_t VulkanAPI::ScorePhysicalDevice(DeviceInfo &indices)
    {
        uint32_t score = 0;
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(indices.device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(indices.device, &deviceFeatures);

        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(indices.device, &memoryProperties);

        if (!deviceFeatures.geometryShader || !CheckDeviceExtensionSupport(indices.device))
            return 0;

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            score += 1000;
        else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            score += 100;

        auto heapsPointer = memoryProperties.memoryHeaps;
        auto heaps = std::vector<VkMemoryHeap>(heapsPointer, heapsPointer + memoryProperties.memoryHeapCount);

        for (const auto &heap : heaps)
        {
            if (heap.flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
            {
                score += heap.size * std::pow(10, -7);
            }
        }

        score += deviceProperties.limits.maxImageDimension2D * 0.02;

        return score;
    }
}