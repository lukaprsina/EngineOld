#include "VulkanAPI/VulkanAPI.h"

namespace eng
{

    template <typename S, typename A>
    bool IsSubset(S &subset, const uint32_t &subsetCount, A &array, const uint32_t &arrayCount) { return false; }

    template <>
    bool Vulkan::IsSubset<std::vector<const char *>, std::vector<VkExtensionProperties>>(std::vector<const char *> &glfwExtensions,
                                                                                            const uint32_t &glfwExtensionCount,
                                                                                            std::vector<VkExtensionProperties> &extensions,
                                                                                            const uint32_t &extensionCount)
    {
        uint32_t i;
        uint32_t j;

        for (i = 0; i < glfwExtensionCount; i++)
        {
            for (j = 0; j < extensionCount; j++)
            {
                if (strcmp(glfwExtensions[i], extensions[j].extensionName) == 0)
                    break;
            }

            if (extensionCount == j)
            {
                return false;
            }
        }

        return true;
    }

    template <>
    bool Vulkan::IsSubset<std::vector<char const *> const, std::vector<VkLayerProperties>>(std::vector<char const *> const &validationLayers,
                                                                                              const uint32_t &validationLayerCount,
                                                                                              std::vector<VkLayerProperties> &availableLayers,
                                                                                              const uint32_t &availableLayerCount)
    {

        uint32_t i;
        uint32_t j;

        for (i = 0; i < validationLayerCount; i++)
        {
            for (j = 0; j < availableLayerCount; j++)
            {
                if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0)
                    break;
            }

            if (availableLayerCount == j)
            {
                return false;
            }
        }

        return true;
    }

    template <>
    bool Vulkan::IsSubset<const std::vector<const char *>, std::vector<VkExtensionProperties>>(const std::vector<const char *> &deviceExtensions,
                                                                                                  const uint32_t &deviceExtensionCount,
                                                                                                  std::vector<VkExtensionProperties> &availableExtensions,
                                                                                                  const uint32_t &availableExtensionCount)
    {
        uint32_t i;
        uint32_t j;

        for (i = 0; i < deviceExtensionCount; i++)
        {
            for (j = 0; j < availableExtensionCount; j++)
            {
                if (strcmp(deviceExtensions[i], availableExtensions[j].extensionName) == 0)
                    break;
            }

            if (availableExtensionCount == j)
            {
                return false;
            }
        }

        return true;
    }
}