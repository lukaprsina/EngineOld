#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class CommandPool
    {
    public:
        CommandPool();
        ~CommandPool();

        VkCommandPool m_VkCommandPool;
    };

    class CommandBuffers
    {
    public:
        CommandBuffers();
        ~CommandBuffers();

        std::vector<VkCommandBuffer> m_VkCommandBuffers;
    };
}