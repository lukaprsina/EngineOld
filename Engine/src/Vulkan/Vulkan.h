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

    struct DeviceInfo
    {
        VkPhysicalDevice device;
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isOK()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
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
        static void Shutdown() { return Get().IShutdown(); }
        static void OnUpdate() { return Get().IOnUpdate(); }
        static void OnWindowResize(WindowResizeEvent &e) { return Get().IOnWindowResize(e); }

        static bool AreValidationLayersEnabled();
        static bool CheckValidationLayerSupport(const std::vector<const char *> &validationLayers);

        template <typename S, typename A>
        static bool IsSubset(S &subset, const uint32_t &subsetCount, A &array, const uint32_t &arrayCount);

        const std::vector<const char *> DeviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        const std::vector<const char *> ValidationLayers{"VK_LAYER_KHRONOS_validation"};

        VkSurfaceKHR surface;

        Settings settings;
        // const std::vector<Vertex> m_Vertices;

        std::unique_ptr<Instance> instance;
        std::unique_ptr<DebugMessenger> debugMessenger;
        std::unique_ptr<PhysicalDevice> physicalDevice;
        /* std::unique_ptr<LogicalDevice> m_LogicalDevice;
        std::unique_ptr<SwapChain> m_SwapChain;
        std::unique_ptr<ImageViews> m_ImageViews;
        std::unique_ptr<RenderPass> m_RenderPass;
        std::unique_ptr<GraphicsPipeline> m_GraphicsPipeline;
        std::unique_ptr<Framebuffers> m_Framebuffers;
        std::unique_ptr<CommandPool> m_CommandPool;
        std::unique_ptr<CommandBuffers> m_CommandBuffers;
        std::unique_ptr<SyncObjects> m_SyncObjects; */

        /* Instance *Instance;
        DebugMessenger *DebugMessenger;
        PhysicalDevice *m_PhysicalDevice;
        LogicalDevice *m_LogicalDevice;
        SwapChain *m_SwapChain;
        ImageViews *m_ImageViews;
        RenderPass *m_RenderPass;
        GraphicsPipeline *m_GraphicsPipeline;
        Framebuffers *m_Framebuffers;
        CommandPool *m_CommandPool;
        CommandBuffers *m_CommandBuffers;
        SyncObjects *m_SyncObjects; */

    private:
        void IInit();
        void IShutdown();
        void IOnUpdate();
        void IOnWindowResize(WindowResizeEvent &e);
    };
}
