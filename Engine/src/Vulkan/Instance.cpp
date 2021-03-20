#include "Vulkan/Instance.h"
#include "Vulkan/DebugMessenger.h"
#include "version.h"

namespace eng
{
    Instance::Instance()
    {
        auto settings = Vulkan::Get().settings;

        auto debugCreateInfo = Vulkan::Get().debugMessenger->Populate();

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        appInfo.pApplicationName = settings.Name.c_str();
        appInfo.pEngineName = "Engine";

        appInfo.applicationVersion = VK_MAKE_VERSION(settings.VersionMayor, settings.VersionMinor, settings.VersionPatch);
        appInfo.engineVersion = VK_MAKE_VERSION(ENG_VERSION_MAJOR, ENG_VERSION_MINOR, ENG_VERSION_PATCH);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        std::vector<const char *> requiredExtensions = GetInstanceExtensions();

        createInfo.enabledExtensionCount = requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        createInfo.enabledExtensionCount = requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        if (Vulkan::AreValidationLayersEnabled() && !Vulkan::CheckValidationLayerSupport(Vulkan::Get().ValidationLayers))
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        createInfo.pNext = &debugCreateInfo;

        if (Vulkan::AreValidationLayersEnabled())
        {
            createInfo.enabledLayerCount = Vulkan::Get().ValidationLayers.size();
            createInfo.ppEnabledLayerNames = Vulkan::Get().ValidationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_VkInstance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }
    }

    Instance::~Instance()
    {
        vkDestroyInstance(m_VkInstance, nullptr);
    }

    std::vector<const char *> Instance::GetInstanceExtensions()
    {
        const char **glfwExtensions;
        uint32_t glfwExtensionsCount = 0;

        GLFW::GetRequiredInstanceExtensions(glfwExtensions, glfwExtensionsCount);
        std::vector<const char *> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

        if (Vulkan::AreValidationLayersEnabled())
        {
            requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        uint32_t allExtensionsCount = 0;
        std::vector<VkExtensionProperties> allExtensions;

        vkEnumerateInstanceExtensionProperties(nullptr, &allExtensionsCount, nullptr);
        allExtensions.resize(allExtensionsCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &allExtensionsCount, allExtensions.data());

        bool areExtensionsSupported = Vulkan::IsSubset(requiredExtensions, requiredExtensions.size(), allExtensions, allExtensionsCount);

        if (!areExtensionsSupported)
        {
            throw std::runtime_error("extensions not found!");
        }

        return requiredExtensions;
    }
}