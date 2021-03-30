#include "VulkanAPI/Device.h"
#include "VulkanAPI/Instance.h"

namespace eng
{
    PhysicalDevice::PhysicalDevice()
    {
        uint32_t deviceCount = 0;
        std::vector<VkPhysicalDevice> devices;

        vkEnumeratePhysicalDevices(Vulkan::Get()->instance->m_VkInstance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        devices.resize(deviceCount);
        vkEnumeratePhysicalDevices(Vulkan::Get()->instance->m_VkInstance, &deviceCount, devices.data());

        std::multimap<int, DeviceInfo> candidates;

        for (const auto &device : devices)
        {
            DeviceInfo indices = GetQueueFamilies(device);
            int score = ScorePhysicalDevice(indices);

            candidates.insert(std::make_pair(score, indices));
        }

        if (Vulkan::Get()->settings.GPU.has_value())
        {
            for (auto &candidate : candidates)
            {
                if (candidate.second.device == Vulkan::Get()->settings.GPU.value())
                {
                    if (candidate.first != 0)
                        GPUProperties = candidate.second;
                    else
                        throw std::runtime_error("the selected GPU isn't suitable!");
                }
            }
        }
        else if (candidates.rbegin()->first > 0)
        {
            GPUProperties = candidates.rbegin()->second;
        }
        else
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    PhysicalDevice::~PhysicalDevice()
    {
    }

    DeviceInfo PhysicalDevice::GetQueueFamilies(const VkPhysicalDevice &device)
    {
        DeviceInfo indices;
        uint32_t queueFamilyCount = 0;

        std::vector<VkQueueFamilyProperties> queueFamilies;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        indices.device = device;

        uint32_t score = 0;
        struct supportedIndices
        {
            int i;
            VkBool32 graphicsFamily = false;
            VkBool32 presentFamily = false;
        };

        std::multimap<int, supportedIndices> scoredFamilies;

        for (uint32_t i = 0; i < queueFamilyCount; i++)
        {
            supportedIndices support;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Vulkan::Get()->instance->m_VkSurface, &support.presentFamily);

            if (support.presentFamily)
                score++;

            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                support.graphicsFamily = true;
                score++;
            }

            support.i = i;

            scoredFamilies.insert(std::make_pair(score, support));
            score = 0;
        }

        if (scoredFamilies.rbegin()->first == 2)
        {
            indices.graphicsFamily = scoredFamilies.rbegin()->second.i;
            indices.presentFamily = scoredFamilies.rbegin()->second.i;

            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &QueuePriority;

            m_VkQueueCreateInfos.emplace_back(queueCreateInfo);
        }

        else if (scoredFamilies.rbegin()->first == 1 && queueFamilyCount > 2)
        {
            supportedIndices support;
            for (auto iter = scoredFamilies.rbegin(); iter != scoredFamilies.rend(); ++iter)
            {
                if (iter->second.graphicsFamily)
                {
                    indices.graphicsFamily = iter->second.i;
                    support.graphicsFamily = true;
                }

                if (iter->second.presentFamily)
                {
                    indices.presentFamily = iter->second.i;
                    support.presentFamily = true;
                }
            }

            if (!support.graphicsFamily || !support.presentFamily)
            {
                throw std::runtime_error("failed to find a suitable GPU!");
            }

            VkDeviceQueueCreateInfo presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            presentInfo.queueFamilyIndex = indices.presentFamily.value();
            presentInfo.queueCount = 1;
            presentInfo.pQueuePriorities = &QueuePriority;

            VkDeviceQueueCreateInfo graphicsInfo{};
            graphicsInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            graphicsInfo.queueFamilyIndex = indices.graphicsFamily.value();
            graphicsInfo.queueCount = 1;
            graphicsInfo.pQueuePriorities = &QueuePriority;

            m_VkQueueCreateInfos.emplace_back(presentInfo);
            m_VkQueueCreateInfos.emplace_back(graphicsInfo);
        }

        else
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        return indices;
    }

    bool PhysicalDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice &device)
    {
        uint32_t extensionCount;
        std::vector<VkExtensionProperties> availableExtensions;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        availableExtensions.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        return Vulkan::IsSubset(Vulkan::Get()->DeviceExtensions, Vulkan::Get()->DeviceExtensions.size(), availableExtensions, availableExtensions.size());
    }

    uint32_t PhysicalDevice::ScorePhysicalDevice(const DeviceInfo &indices)
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

    LogicalDevice::LogicalDevice()
    {
        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = Vulkan::Get()->physicalDevice->m_VkQueueCreateInfos.size();
        createInfo.pQueueCreateInfos = Vulkan::Get()->physicalDevice->m_VkQueueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = Vulkan::Get()->DeviceExtensions.size();
        createInfo.ppEnabledExtensionNames = Vulkan::Get()->DeviceExtensions.data();

        if (Vulkan::AreValidationLayersEnabled())
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(Vulkan::Get()->ValidationLayers.size());
            createInfo.ppEnabledLayerNames = Vulkan::Get()->ValidationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(Vulkan::Get()->physicalDevice->GPUProperties.device, &createInfo, nullptr, &m_VkLogicalDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(m_VkLogicalDevice, Vulkan::Get()->physicalDevice->GPUProperties.graphicsFamily.value(), 0, &m_VkGraphicsQueue);
        vkGetDeviceQueue(m_VkLogicalDevice, Vulkan::Get()->physicalDevice->GPUProperties.presentFamily.value(), 0, &m_VkPresentQueue);
    }

    LogicalDevice::~LogicalDevice()
    {
        vkDestroyDevice(m_VkLogicalDevice, nullptr);
    }
}