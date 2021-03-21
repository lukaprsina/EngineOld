#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class LogicalDevice
    {
    public:
        LogicalDevice();
        ~LogicalDevice();

        VkDevice m_VkLogicalDevice;
        VkQueue m_VkGraphicsQueue;
        VkQueue m_VkPresentQueue;
    };
}