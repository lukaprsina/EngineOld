#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class ImageViews
    {
    public:
        ImageViews();
        ~ImageViews();

        std::vector<VkImageView> m_VkSwapChainImageViews;
    };
}