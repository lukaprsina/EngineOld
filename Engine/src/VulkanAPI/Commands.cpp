#include "VulkanAPI/Commands.h"
#include "VulkanAPI/Framebuffers.h"
#include "VulkanAPI/Device.h"
#include "VulkanAPI/SwapChain.h"
#include "VulkanAPI/GraphicsPipeline.h"
#include "VulkanAPI/Buffers.h"
#include "VulkanAPI/Descriptors.h"

namespace eng
{
    CommandPool::CommandPool()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = Vulkan::Get()->physicalDevice->GPUProperties.graphicsFamily.value();

        if (vkCreateCommandPool(Vulkan::Get()->logicalDevice->m_VkLogicalDevice, &poolInfo, nullptr, &m_VkCommandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(Vulkan::Get()->logicalDevice->m_VkLogicalDevice, m_VkCommandPool, nullptr);
    }

    CommandBuffers::CommandBuffers()
    {
        auto swapChainExtent = Vulkan::Get()->swapChain->m_VkSwapChainExtent;
        auto swapChainFramebuffers = Vulkan::Get()->framebuffers->m_VkSwapChainFramebuffers;

        m_VkCommandBuffers.resize(swapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = Vulkan::Get()->commandPool->m_VkCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_VkCommandBuffers.size());

        if (vkAllocateCommandBuffers(Vulkan::Get()->logicalDevice->m_VkLogicalDevice, &allocInfo, m_VkCommandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (size_t i = 0; i < m_VkCommandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(m_VkCommandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = Vulkan::Get()->renderPass->m_VkRenderPass;
            renderPassInfo.framebuffer = swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChainExtent;

            VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(m_VkCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(m_VkCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, Vulkan::Get()->graphicsPipeline->m_VkGraphicsPipeline);

            VkBuffer vertexBuffers[] = {Vulkan::Get()->vertexBuffer->m_VkVertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(m_VkCommandBuffers[i], 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(m_VkCommandBuffers[i],
                                 Vulkan::Get()->indexBuffer->m_VkIndexBuffer,
                                 0, VK_INDEX_TYPE_UINT16);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(swapChainExtent.width);
            viewport.height = static_cast<float>(swapChainExtent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(m_VkCommandBuffers[i], 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swapChainExtent;
            vkCmdSetScissor(m_VkCommandBuffers[i], 0, 1, &scissor);

            vkCmdBindDescriptorSets(m_VkCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, Vulkan::Get()->graphicsPipeline->m_VkPipelineLayout, 0, 1, &Vulkan::Get()->descriptorSets->m_VkDescriptorSets[i], 0, nullptr);

            vkCmdDrawIndexed(m_VkCommandBuffers[i], Vulkan::Get()->m_Indices.size(), 1, 0, 0, 0);

            vkCmdEndRenderPass(m_VkCommandBuffers[i]);

            if (vkEndCommandBuffer(m_VkCommandBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }

    CommandBuffers::~CommandBuffers()
    {
    }
}