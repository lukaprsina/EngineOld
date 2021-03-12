#include "Vulkan/VulkanAPI.h"

namespace eng
{
    std::vector<const char *> VulkanAPI::getRequiredExtensions(GLFW &glfw)
    {
        const char **glfwExtensions;
        uint32_t glfwExtensionsCount = 0;

        glfw.GetRequiredInstanceExtensions(glfwExtensions, glfwExtensionsCount);
        std::vector<const char *> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

        if (enableValidationLayers)
        {
            requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        uint32_t allExtensionsCount = 0;
        std::vector<VkExtensionProperties> allExtensions;

        vkEnumerateInstanceExtensionProperties(nullptr, &allExtensionsCount, nullptr);
        allExtensions.resize(allExtensionsCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &allExtensionsCount, allExtensions.data());

        bool areExtensionsSupported = IsSubset(requiredExtensions, requiredExtensions.size(), allExtensions, allExtensionsCount);

        if (!areExtensionsSupported)
        {
            throw std::runtime_error("extensions not found!");
        }

        return requiredExtensions;
    }
}