#pragma once

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"

namespace eng
{
    class VulkanAPI
    {
    public:
        VulkanAPI(GLFW &glfw);
        ~VulkanAPI();

    private:
        VkInstance instance;
        void CreateInstance(GLFW &glfw);

        template <typename S, typename A>
        bool IsSubset(S &subset, const uint32_t subsetCount, A &array, const uint32_t arrayCount);

        std::vector<const char *> getRequiredExtensions(GLFW &glfw);

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif

        const std::vector<const char *> validationLayers;
        bool checkValidationLayerSupport();

        // TODO: another class for loaded functions
        VkDebugUtilsMessengerEXT debugMessenger;
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        void PopulateDebugMessenger();
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
        void SetupDebugMessenger();

        VkPhysicalDevice physicalDevice;
        void PickPhysicalDevice();
        uint32_t ScorePhysicalDevice(VkPhysicalDevice device);

        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;

            bool isOkay()
            {
                return graphicsFamily.has_value();
            }
        };

        struct QueueFamilyIndices physicalDeviceIndices;
        uint32_t ScoreQueueFamilies(VkPhysicalDevice device, QueueFamilyIndices &indices);

        VkDevice logicalDevice;
        void CreateLogicalDevice();

        void Cleanup();
    };
}