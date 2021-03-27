#include "Vulkan/Descriptors.h"
#include "Vulkan/Buffers.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/Device.h"

namespace eng
{
    DescriptorSetLayout::DescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;

        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &layoutInfo, nullptr, &m_VkDescriptorSetLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(Vulkan::Get().logicalDevice->m_VkLogicalDevice, m_VkDescriptorSetLayout, nullptr);
    }

    DescriptorPool::DescriptorPool()
    {
        auto swapChainImages = Vulkan::Get().swapChain->m_VkSwapChainImages;
        auto logicalDevice = Vulkan::Get().logicalDevice->m_VkLogicalDevice;

        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

        if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &m_VkDescriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    DescriptorPool::~DescriptorPool()
    {
        vkDestroyDescriptorPool(Vulkan::Get().logicalDevice->m_VkLogicalDevice, m_VkDescriptorPool, nullptr);
    }

    DescriptorSets::DescriptorSets()
    {
        auto swapChainImages = Vulkan::Get().swapChain->m_VkSwapChainImages;
        auto logicalDevice = Vulkan::Get().logicalDevice->m_VkLogicalDevice;

        std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), Vulkan::Get().descriptorSetLayout->m_VkDescriptorSetLayout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = Vulkan::Get().descriptorPool->m_VkDescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
        allocInfo.pSetLayouts = layouts.data();

        m_VkDescriptorSets.resize(swapChainImages.size());
        if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, m_VkDescriptorSets.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < swapChainImages.size(); i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = Vulkan::Get().uniformBuffer->m_VkUniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(MVP);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = m_VkDescriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;

            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);
        }
    }

    DescriptorSets::~DescriptorSets()
    {
    }
}