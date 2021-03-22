#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class Framebuffers
    {
    public:
        Framebuffers();
        ~Framebuffers();

        std::vector<VkFramebuffer> m_VkSwapChainFramebuffers;
    };
}