#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class GraphicsPipeline
    {
    public:
        GraphicsPipeline();
        ~GraphicsPipeline();

        VkPipelineLayout m_VkPipelineLayout;
        VkPipeline m_VkGraphicsPipeline;

        VkShaderModule CreateShaderModule(const std::vector<char> &code);
        std::vector<char> *ReadFile(const std::string &filename);
    };
}