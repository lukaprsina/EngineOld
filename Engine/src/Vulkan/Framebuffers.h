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

    class RenderPass
    {
    public:
        RenderPass();
        ~RenderPass();

        VkRenderPass m_VkRenderPass;
    };

    class ImageViews
    {
    public:
        ImageViews();
        ~ImageViews();

        std::vector<VkImageView> m_VkSwapChainImageViews;
    };
}