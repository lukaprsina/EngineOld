#include "Vulkan/SyncObjects.h"
#include "Vulkan/LogicalDevice.h"
#include "Vulkan/SwapChain.h"

namespace eng
{
    SyncObjects::SyncObjects()
    {
        m_VkImageAvailableSemaphores.resize(Vulkan::Get().MAX_FRAMES_IN_FLIGHT);
        m_VkRenderFinishedSemaphores.resize(Vulkan::Get().MAX_FRAMES_IN_FLIGHT);
        m_VkInFlightFences.resize(Vulkan::Get().MAX_FRAMES_IN_FLIGHT);
        m_VkImagesInFlight.resize(Vulkan::Get().swapChain->m_VkSwapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < Vulkan::Get().MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &semaphoreInfo, nullptr, &m_VkImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &semaphoreInfo, nullptr, &m_VkRenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &fenceInfo, nullptr, &m_VkInFlightFences[i]) != VK_SUCCESS)
            {

                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }

        /* VkSemaphoreTypeCreateInfo timelineCreateInfo;
        timelineCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
        timelineCreateInfo.pNext = NULL;
        timelineCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
        timelineCreateInfo.initialValue = 0;

        VkSemaphoreCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        createInfo.pNext = &timelineCreateInfo;
        createInfo.flags = 0;

        vkCreateSemaphore(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &createInfo, NULL, &m_VkTimeline); */
    }

    SyncObjects::~SyncObjects()
    {
        for (size_t i = 0; i < Vulkan::Get().MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(Vulkan::Get().logicalDevice->m_VkLogicalDevice, m_VkRenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(Vulkan::Get().logicalDevice->m_VkLogicalDevice, m_VkImageAvailableSemaphores[i], nullptr);
            vkDestroyFence(Vulkan::Get().logicalDevice->m_VkLogicalDevice, m_VkInFlightFences[i], nullptr);
        }
    }
}