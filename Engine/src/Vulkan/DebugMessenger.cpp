#include "Vulkan/DebugMessenger.h"
#include "Vulkan/Instance.h"
#include "Core/Log.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
    eng::Settings *settings = reinterpret_cast<eng::Settings *>(pUserData);

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