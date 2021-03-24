#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class Instance
    {
    public:
        Instance();
        ~Instance();

        VkInstance m_VkInstance;
        VkSurfaceKHR m_VkSurface;

    private:
        std::vector<const char *> GetInstanceExtensions();
    };

    class DebugMessenger
    {
    public:
        DebugMessenger();
        ~DebugMessenger();

        VkDebugUtilsMessengerCreateInfoEXT m_DebugCreateInfo;
        VkDebugUtilsMessengerCreateInfoEXT Populate();

        VkResult CreateDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerEXT &debugMessenger, const VkAllocationCallbacks *pAllocator);

    private:
        VkDebugUtilsMessengerEXT m_VkDebugMessenger;
    };
}