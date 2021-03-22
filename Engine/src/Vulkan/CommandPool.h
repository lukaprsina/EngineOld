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
}