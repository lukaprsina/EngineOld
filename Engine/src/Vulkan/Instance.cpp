#include "Vulkan/VulkanAPI.h"
#include "version.h"

namespace eng
{
    void VulkanAPI::CreateInstance()
    {
        // TODO: get app version and name
        PopulateDebugMessenger();
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

        std::vector<const char *> requiredExtensions = GetInstanceExtensions();

        createInfo.enabledExtensionCount = requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        if (enableValidationLayers && !CheckValidationLayerSupport())
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        createInfo.pNext = &debugCreateInfo;
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = validationLayers.size();
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