#include "Vulkan/VulkanAPI.h"
#include "version.h"

namespace eng
{
    void VulkanAPI::CreateInstance(GLFW &glfw)
    {
        // TODO: get app version and name
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        appInfo.pApplicationName = "Engine";
        appInfo.pEngineName = "Engine";

        appInfo.applicationVersion = VK_MAKE_VERSION(ENG_VERSION_MAJOR, ENG_VERSION_MINOR, ENG_VERSION_PATCH);
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        const char **glfwExtensions;
        uint32_t glfwExtensionCount = 0;

        glfw.GetRequiredInstanceExtensions(glfwExtensions, glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        // EXTENSIONS
        uint32_t extensionCount = 0;
        std::vector<VkExtensionProperties> extensions;

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        extensions.resize(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        bool areExtensionsSupported = IsSubset(glfwExtensions, glfwExtensionCount, extensions, extensionCount);

        if (!areExtensionsSupported)
        {
            throw std::runtime_error("extensions not found!");
        }

        // VALIDATION LAYERS
        uint32_t layerCount;
        std::vector<VkLayerProperties> availableLayers;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        availableLayers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        bool areValidationLayersSupported = IsSubset(validationLayers, validationLayers.size(), availableLayers, layerCount);

        if (enableValidationLayers && !areValidationLayersSupported)
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }
    }
}