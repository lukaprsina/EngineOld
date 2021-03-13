#pragma once

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"

namespace eng
{
    struct DeviceIndices
    {
        VkPhysicalDevice device;
        std::optional<uint32_t> graphicsFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value();
        }
    };

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

        DeviceIndices GPUProperties;
        void PickPhysicalDevice();

        DeviceIndices GetQueueFamilies(VkPhysicalDevice device);
        uint32_t ScorePhysicalDevice(DeviceIndices &indices);

        VkDevice logicalDevice;
        VkQueue graphicsQueue;
        void CreateLogicalDevice();

        void Cleanup();
    };
}