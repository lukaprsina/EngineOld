#include "Vulkan/Vulkan.h"
#include "Vulkan/ImageViews.h"
#include "Vulkan/Device.h"
#include "Vulkan/SwapChain.h"

namespace eng
{
    ImageViews::ImageViews()
    {
        auto swapChainImages = Vulkan::Get().swapChain->m_VkSwapChainImages;
        m_VkSwapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];

            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = Vulkan::Get().swapChain->m_VkSwapChainImageFormat;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &createInfo, nullptr, &m_VkSwapChainImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }

    ImageViews::~ImageViews()
    {
        for (auto imageView : m_VkSwapChainImageViews)
        {
            vkDestroyImageView(Vulkan::Get().logicalDevice->m_VkLogicalDevice, imageView, nullptr);
        }
    }
}