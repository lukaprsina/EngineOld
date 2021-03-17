#pragma once

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"
#include "Core/Log.h"
#include <optional>

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

    struct Settings
    {
        std::string Name = "Application";
        uint32_t VersionMayor = 1;
        uint32_t VersionMinor = 0;
        uint32_t VersionPatch = 0;

        uint32_t VulkanMessageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        uint32_t VulkanMessageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        VkPhysicalDevice GPU = 0;
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
        static void OnUpdate() { return Get().IOnUpdate(); }

    private:
        void IInit();
        void IShutdown();
        void IOnUpdate();

        VkInstance m_Instance;
        void CreateInstance();

        Settings m_Settings;

        template <typename S, typename A>
        bool IsSubset(S &subset, const uint32_t &subsetCount, A &array, const uint32_t &arrayCount);

        std::vector<const char *> GetInstanceExtensions();

#ifdef NDEBUG
        const bool m_EnableValidationLayers = false;
#else
        const bool m_EnableValidationLayers = true;
#endif

        const std::vector<const char *> m_DeviceExtensions;
        const std::vector<const char *> m_ValidationLayers;
        bool CheckValidationLayerSupport();

        // TODO: another class for loaded functions
        // TODO: & for all function calls
        // TODO: private members m_ convention

        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkDebugUtilsMessengerCreateInfoEXT m_DebugCreateInfo;
        void PopulateDebugMessenger();
        VkResult CreateDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerEXT &debugMessenger, const VkAllocationCallbacks *pAllocator);
        void SetupDebugMessenger();

        VkSurfaceKHR m_Surface;
        void CreateSurface();

        DeviceInfo m_GPUProperties;
        void PickPhysicalDevice();

        const float m_QueuePriority = 1.0f;
        bool CheckDeviceExtensionSupport(const VkPhysicalDevice &device);
        DeviceInfo GetQueueFamilies(const VkPhysicalDevice &device);
        uint32_t ScorePhysicalDevice(const DeviceInfo &indices);

        VkDevice m_LogicalDevice;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
        std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;
        void CreateLogicalDevice();

        VkSwapchainKHR m_SwapChain;
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        VkFormat m_SwapChainImageFormat;
        VkExtent2D m_SwapChainExtent;
        std::vector<VkImage> m_SwapChainImages;
        void QuerySwapChainSupport(DeviceInfo &indices);
        void CreateSwapChain();

        std::vector<VkImageView> m_SwapChainImageViews;
        void CreateImageViews();

        VkRenderPass m_RenderPass;
        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_GraphicsPipeline;
        void CreateRenderPass();

        // TODO: use shaderc
        std::vector<char> *ReadFile(const std::string &filename);
        VkShaderModule CreateShaderModule(const std::vector<char> &code);
        void CreateGraphicsPipeline();

        std::vector<VkFramebuffer> m_SwapChainFramebuffers;
        void CreateFramebuffers();

        VkCommandPool m_CommandPool;
        void CreateCommandPool();

        std::vector<VkCommandBuffer> m_CommandBuffers;
        void CreateCommandBuffers();

        VkSemaphore m_ImageAvailableSemaphore;
        VkSemaphore m_RenderFinishedSemaphore;
        void CreateSemaphores();

        void DrawFrame();

        void Cleanup();
    };
}