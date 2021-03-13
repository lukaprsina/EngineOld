#include "Vulkan/VulkanAPI.h"

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

        uint32_t maxScore = 0;
        uint32_t currentScore;
        VkPhysicalDevice bestDevice;
        QueueFamilyIndices indices;

        for (const auto &device : devices)
        {
            currentScore = ScorePhysicalDevice(device, indices);

            if (!indices.isOkay())
                throw std::runtime_error("the GPU can't run this program!");

            if (currentScore > maxScore)
            {
                bestDevice = device;
                maxScore = currentScore;
            }
        }

        if (maxScore == 0)
            throw std::runtime_error("failed to find a suitable GPU!");
        else
            physicalDevice = bestDevice;
    }

    uint32_t VulkanAPI::ScorePhysicalDevice(VkPhysicalDevice device, QueueFamilyIndices &indices)
    {
        // TODO: score more parameters
        uint32_t score = 0;
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

        if (!deviceFeatures.geometryShader)
            return 0;

        score += ScoreQueueFamilies(device, indices);

        if (deviceProperties.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            score += 1000;
        else if (deviceProperties.deviceType = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
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