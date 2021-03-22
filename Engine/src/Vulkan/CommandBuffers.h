#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class CommandBuffers
    {
    public:
        CommandBuffers();
        ~CommandBuffers();

        std::vector<VkCommandBuffer> m_VkCommandBuffers;
    };
}