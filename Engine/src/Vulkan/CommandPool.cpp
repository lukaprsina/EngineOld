#include "Vulkan/CommandPool.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/LogicalDevice.h"

namespace eng
{
    CommandPool::CommandPool()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = Vulkan::Get().physicalDevice->GPUProperties.graphicsFamily.value();

        if (vkCreateCommandPool(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &poolInfo, nullptr, &m_VkCommandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(Vulkan::Get().logicalDevice->m_VkLogicalDevice, m_VkCommandPool, nullptr);
    }
}