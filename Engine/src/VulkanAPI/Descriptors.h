#pragma once

#include "VulkanAPI/VulkanAPI.h"

namespace eng
{
    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout();
        ~DescriptorSetLayout();

        VkDescriptorSetLayout m_VkDescriptorSetLayout;
    };

    class DescriptorPool
    {
    public:
        DescriptorPool();
        ~DescriptorPool();

        VkDescriptorPool m_VkDescriptorPool;
    };

    class DescriptorSets
    {
    public:
        DescriptorSets();
        ~DescriptorSets();

        std::vector<VkDescriptorSet> m_VkDescriptorSets;
    };
}