#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class RenderPass
    {
    public:
        RenderPass();
        ~RenderPass();

        VkRenderPass m_VkRenderPass;
    };
}