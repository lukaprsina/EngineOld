#pragma once

#include "pch.h"

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"
#include "glm/glm.hpp"
#include "vk_mem_alloc.h"

#include "Core/Log.h"
#include "Events/ApplicationEvent.h"

namespace eng
{
    class Instance;
    class DebugMessenger;
    class PhysicalDevice;
    class LogicalDevice;
    class SwapChain;
    class ImageViews;
    class RenderPass;
    class GraphicsPipeline;
    class Framebuffers;
    class CommandPool;
    class CommandBuffers;
    class SyncObjects;

    struct VulkanSettings
    {
        std::string Name = "Application";
        uint32_t VersionMayor = 1;
        uint32_t VersionMinor = 0;
        uint32_t VersionPatch = 0;

        uint32_t VulkanMessageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        uint32_t VulkanMessageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        std::optional<VkPhysicalDevice> GPU;

        std::optional<VkSurfaceFormatKHR> SwapSurfaceFormat;
        std::optional<VkPresentModeKHR> SwapSurfacePresentMode;
    };

    class Vulkan
    {
    public:
        Vulkan();
        ~Vulkan();

        Vulkan(const Vulkan &) = delete;
        Vulkan &operator=(Vulkan const &) = delete;

        static Vulkan &Get()
        {
            static Vulkan instance;
            return instance;
        }

        static void Init() { return Get().IInit(); }
        static void Init(VulkanSettings &s) { return Get().IInit(s); }
        static void Shutdown() { return Get().IShutdown(); }
        static void OnUpdate() { return Get().IOnUpdate(); }
        static void OnWindowResize(WindowResizeEvent &e) { return Get().IOnWindowResize(e); }
        static void ChangeSettings(VulkanSettings &s) { return Get().IChangeSettings(s); }

        static bool AreValidationLayersEnabled();
        static bool CheckValidationLayerSupport(const std::vector<const char *> &validationLayers);

        template <typename S, typename A>
        static bool IsSubset(S &subset, const uint32_t &subsetCount, A &array, const uint32_t &arrayCount);

        const std::vector<const char *> DeviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        const std::vector<const char *> ValidationLayers{"VK_LAYER_KHRONOS_validation"};

        VulkanSettings settings;
        // const std::vector<Vertex> m_Vertices;

        std::unique_ptr<Instance> instance;
        std::unique_ptr<DebugMessenger> debugMessenger;
        std::unique_ptr<PhysicalDevice> physicalDevice;
        std::unique_ptr<LogicalDevice> logicalDevice;
        std::unique_ptr<SwapChain> swapChain;
        std::unique_ptr<ImageViews> imageViews;
        std::unique_ptr<RenderPass> renderPass;
        std::unique_ptr<GraphicsPipeline> graphicsPipeline;
        std::unique_ptr<Framebuffers> framebuffers;
        std::unique_ptr<CommandPool> commandPool;
        std::unique_ptr<CommandBuffers> commandBuffers;
        std::unique_ptr<SyncObjects> syncObjects;

        const size_t MAX_FRAMES_IN_FLIGHT = 2;

    private:
        void IInit();
        void IInit(VulkanSettings &s);
        void IShutdown();
        void IOnUpdate();
        void IOnWindowResize(WindowResizeEvent &e);
        void IChangeSettings(VulkanSettings &s);

        size_t CurrentFrame = 0;
        bool m_FramebufferResized = false;
        void DrawFrame();
    };
}
