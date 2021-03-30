#pragma once

#include "VulkanAPI/VulkanAPI.h"

namespace eng
{
    class SyncObjects
    {
    public:
        SyncObjects();
        ~SyncObjects();

        //VkSemaphore m_VkTimeline;
        std::vector<VkSemaphore> m_VkImageAvailableSemaphores;
        std::vector<VkSemaphore> m_VkRenderFinishedSemaphores;
        std::vector<VkFence> m_VkInFlightFences;
        std::vector<VkFence> m_VkImagesInFlight;
    };
}