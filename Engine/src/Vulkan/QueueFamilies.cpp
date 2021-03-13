#include "Vulkan/VulkanAPI.h"

namespace eng
{
    DeviceIndices VulkanAPI::GetQueueFamilies(VkPhysicalDevice device)
    {
        DeviceIndices indices;
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
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &support.presentFamily);

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

            float queuePriority = 1.0f;

            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.emplace_back(queueCreateInfo);
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

            float queuePriority = 1.0f;

            VkDeviceQueueCreateInfo presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            presentInfo.queueFamilyIndex = indices.presentFamily.value();
            presentInfo.queueCount = 1;
            presentInfo.pQueuePriorities = &queuePriority;

            VkDeviceQueueCreateInfo graphicsInfo{};
            graphicsInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            graphicsInfo.queueFamilyIndex = indices.graphicsFamily.value();
            graphicsInfo.queueCount = 1;
            graphicsInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.emplace_back(presentInfo);
            queueCreateInfos.emplace_back(graphicsInfo);
        }

        else
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        return indices;
    }
}