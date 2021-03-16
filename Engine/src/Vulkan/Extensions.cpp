#include "Vulkan/VulkanAPI.h"

namespace eng
{
    std::vector<const char *> VulkanAPI::GetInstanceExtensions()
    {
        const char **glfwExtensions;
        uint32_t glfwExtensionsCount = 0;

        GLFW::GetRequiredInstanceExtensions(glfwExtensions, glfwExtensionsCount);
        std::vector<const char *> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

        if (m_EnableValidationLayers)
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

    bool VulkanAPI::CheckDeviceExtensionSupport(const VkPhysicalDevice &device)
    {
        uint32_t extensionCount;
        std::vector<VkExtensionProperties> availableExtensions;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        availableExtensions.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        return IsSubset(m_DeviceExtensions, m_DeviceExtensions.size(), availableExtensions, availableExtensions.size());
    }
}