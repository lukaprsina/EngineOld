#include "Vulkan/VulkanAPI.h"

namespace eng
{
    bool VulkanAPI::CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        std::vector<VkLayerProperties> availableLayers;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        availableLayers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        bool validationLayersSupported = IsSubset(m_ValidationLayers, m_ValidationLayers.size(), availableLayers, layerCount);
        return (validationLayersSupported);
    }
}