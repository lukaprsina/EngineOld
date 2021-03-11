#include "Vulkan/VulkanAPI.h"

namespace eng
{

    template <typename S, typename A>
    bool IsSubset(S &subset, const uint16_t &subsetCount, A &array, const uint16_t &arrayCount) { return false; }

    template <>
    bool VulkanAPI::IsSubset<const char **, std::vector<VkExtensionProperties>>(const char **&glfwExtensions,
                                                                                const uint16_t &glfwExtensionCount,
                                                                                std::vector<VkExtensionProperties> &extensions,
                                                                                const uint16_t &extensionCount)
    {
        int i;
        int j;

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
    bool VulkanAPI::IsSubset<const std::vector<const char *>, std::vector<VkLayerProperties>>(const std::vector<const char *> &validationLayers,
                                                                                              const uint16_t &validationLayerCount,
                                                                                              std::vector<VkLayerProperties> &availableLayers,
                                                                                              const uint16_t &availableLayerCount)
    {
        {
            int i;
            int j;

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
    }

}