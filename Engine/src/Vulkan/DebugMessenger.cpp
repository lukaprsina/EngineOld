#include "Vulkan/Vulkan.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
    eng::Settings *settings = reinterpret_cast<eng::Settings *>(pUserData);

    if ((messageSeverity >= settings->VulkanMessageSeverity) &&
        (messageType >= settings->VulkanMessageType))

        std::cout << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

namespace eng
{
    void Vulkan::SetupDebugMessenger()
    {
        if (!m_EnableValidationLayers)
            return;

        if (CreateDebugUtilsMessengerEXT(m_Instance, &m_DebugCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    void Vulkan::PopulateDebugMessenger()
    {
        m_DebugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        m_DebugCreateInfo.messageSeverity = m_Settings.VulkanMessageSeverity;
        m_DebugCreateInfo.messageType = m_Settings.VulkanMessageType;
        m_DebugCreateInfo.pfnUserCallback = debugCallback;
        m_DebugCreateInfo.pUserData = &m_Settings;
    }

    VkResult Vulkan::CreateDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
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

    void Vulkan::DestroyDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerEXT &debugMessenger, const VkAllocationCallbacks *pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }
}