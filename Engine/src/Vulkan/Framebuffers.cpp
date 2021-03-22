#include "Vulkan/Framebuffers.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/ImageViews.h"
#include "Vulkan/RenderPass.h"
#include "Vulkan/LogicalDevice.h"

namespace eng
{
    Framebuffers::Framebuffers()
    {
        auto imageViews = Vulkan::Get().imageViews->m_VkSwapChainImageViews;
        auto swapChainExtent = Vulkan::Get().swapChain->m_VkSwapChainExtent;

        m_VkSwapChainFramebuffers.resize(imageViews.size());
        for (size_t i = 0; i < imageViews.size(); i++)
        {
            VkImageView attachments[] = {
                imageViews[i]};

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = Vulkan::Get().renderPass->m_VkRenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &framebufferInfo, nullptr, &m_VkSwapChainFramebuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    Framebuffers::~Framebuffers()
    {
        for (auto framebuffer : m_VkSwapChainFramebuffers)
        {
            vkDestroyFramebuffer(Vulkan::Get().logicalDevice->m_VkLogicalDevice, framebuffer, nullptr);
        }
    }
}