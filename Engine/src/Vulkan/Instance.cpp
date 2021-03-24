#include "Vulkan/Instance.h"
#include "Core/Log.h"
#include "version.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
    eng::VulkanSettings *settings = reinterpret_cast<eng::VulkanSettings *>(pUserData);

    if ((messageSeverity >= settings->VulkanMessageSeverity) &&
        (messageType >= settings->VulkanMessageType))
    {
        switch (messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            ENG_CORE_TRACE(pCallbackData->pMessage);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            ENG_CORE_TRACE(pCallbackData->pMessage);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            ENG_CORE_WARN(pCallbackData->pMessage);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            ENG_CORE_ERROR(pCallbackData->pMessage);
            break;

        default:
            break;
        }
    }

    return VK_FALSE;
}

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
        vkDestroySurfaceKHR(m_VkInstance, m_VkSurface, nullptr);
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

    DebugMessenger::DebugMessenger()
        : m_VkDebugMessenger{}
    {
        if (!Vulkan::AreValidationLayersEnabled())
            return;

        m_DebugCreateInfo = Populate();

        if (CreateDebugUtilsMessengerEXT(Vulkan::Get().instance->m_VkInstance, &m_DebugCreateInfo, nullptr, &m_VkDebugMessenger) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    DebugMessenger::~DebugMessenger()
    {
        if (Vulkan::AreValidationLayersEnabled())
        {
            DestroyDebugUtilsMessengerEXT(Vulkan::Get().instance->m_VkInstance, m_VkDebugMessenger, nullptr);
        }
    }

    VkDebugUtilsMessengerCreateInfoEXT DebugMessenger::Populate()
    {
        auto settings = Vulkan::Get().settings;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = settings.VulkanMessageSeverity;
        debugCreateInfo.messageType = settings.VulkanMessageType;
        debugCreateInfo.pfnUserCallback = debugCallback;
        debugCreateInfo.pUserData = static_cast<void *>(&settings);

        return debugCreateInfo;
    }

    VkResult DebugMessenger::CreateDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DebugMessenger::DestroyDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerEXT &debugMessenger, const VkAllocationCallbacks *pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }
}