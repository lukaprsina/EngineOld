#include "Vulkan/VulkanAPI.h"
#include "version.h"

namespace eng
{
    void VulkanAPI::createInstance(GLFW &glfw)
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

        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }
    }
}