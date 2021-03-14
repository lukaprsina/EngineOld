#pragma once

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"
#include "Core/Log.h"

namespace eng
{
    struct DeviceInfo
    {
        VkPhysicalDevice device;
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        bool isOK()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class VulkanAPI
    {
    public:
        VulkanAPI();
        ~VulkanAPI();

        VulkanAPI(const VulkanAPI &) = delete;
        VulkanAPI &operator=(VulkanAPI const &) = delete;

        static VulkanAPI &Get()
        {
            static VulkanAPI instance;
            return instance;
        }

        static void Init() { return Get().IInit(); }
        static void Shutdown() { return Get().IShutdown(); }

    private:
        void IInit();
        void IShutdown();

        VkInstance instance;
        void CreateInstance();

        template <typename S, typename A>
        bool IsSubset(S &subset, const uint32_t subsetCount, A &array, const uint32_t arrayCount);

        std::vector<const char *> GetInstanceExtensions();

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif

        const std::vector<const char *> deviceExtensions;
        const std::vector<const char *> validationLayers;
        bool CheckValidationLayerSupport();

        // TODO: another class for loaded functions
        // TODO: & for all function calls
        // TODO: private members m_ convention

        VkDebugUtilsMessengerEXT debugMessenger;
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        void PopulateDebugMessenger();
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
        void SetupDebugMessenger();

        VkSurfaceKHR surface;
        void CreateSurface();

        DeviceInfo GPUProperties;
        void PickPhysicalDevice();

        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        DeviceInfo GetQueueFamilies(VkPhysicalDevice device);
        uint32_t ScorePhysicalDevice(DeviceInfo &indices);

        VkDevice logicalDevice;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        void CreateLogicalDevice();

        VkSwapchainKHR swapChain;
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void QuerySwapChainSupport(DeviceInfo &indices);
        void CreateSwapChain();

        void Cleanup();
    };
}