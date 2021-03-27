#pragma once

#include "pch.h"

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"
#include "vk_mem_alloc.h"

#define GLM_FORCE_RADIANS
#define GLM_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    class DescriptorSetLayout;
    class GraphicsPipeline;
    class Framebuffers;
    class CommandPool;
    class Memory;
    class VertexBuffer;
    class IndexBuffer;
    class UniformBuffer;
    class DescriptorPool;
    class DescriptorSets;
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

    struct Vertex
    {
        glm::vec2 Position;
        glm::vec3 Color;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, Position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, Color);
            return attributeDescriptions;
        }
    };

    struct MVP
    {
        alignas(16) glm::mat4 Model;
        alignas(16) glm::mat4 View;
        alignas(16) glm::mat4 Proj;
    };

    class Vulkan
    {
    public:
        Vulkan();
        ~Vulkan();

        Vulkan(const Vulkan &) = delete;
        Vulkan &operator=(Vulkan const &) = delete;

        static Vulkan *&Get()
        {
            static Vulkan *instance = new Vulkan;
            return instance;
        }

        static void Init() { return Get()->IInit(); }
        static void Init(VulkanSettings &s) { return Get()->IInit(s); }
        static void Shutdown() { return Get()->IShutdown(); }
        static void OnUpdate(float &time) { return Get()->IOnUpdate(time); }
        static void OnWindowResize(WindowResizeEvent &e) { return Get()->IOnWindowResize(e); }
        static void ChangeSettings(VulkanSettings &s) { return Get()->IChangeSettings(s); }

        static bool AreValidationLayersEnabled();
        static bool CheckValidationLayerSupport(const std::vector<const char *> &validationLayers);

        template <typename S, typename A>
        static bool IsSubset(S &subset, const uint32_t &subsetCount, A &array, const uint32_t &arrayCount);

        const std::vector<const char *> DeviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        const std::vector<const char *> ValidationLayers{"VK_LAYER_KHRONOS_validation"};

        VulkanSettings settings;
        const std::vector<Vertex> m_Vertices;
        const std::vector<uint16_t> m_Indices;

        std::unique_ptr<Instance> instance;
        std::unique_ptr<DebugMessenger> debugMessenger;
        std::unique_ptr<PhysicalDevice> physicalDevice;
        std::unique_ptr<LogicalDevice> logicalDevice;
        std::unique_ptr<SwapChain> swapChain;
        std::unique_ptr<ImageViews> imageViews;
        std::unique_ptr<RenderPass> renderPass;
        std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
        std::unique_ptr<GraphicsPipeline> graphicsPipeline;
        std::unique_ptr<Framebuffers> framebuffers;
        std::unique_ptr<CommandPool> commandPool;
        std::unique_ptr<Memory> memory;
        std::unique_ptr<VertexBuffer> vertexBuffer;
        std::unique_ptr<IndexBuffer> indexBuffer;
        std::unique_ptr<UniformBuffer> uniformBuffer;
        std::unique_ptr<DescriptorPool> descriptorPool;
        std::unique_ptr<DescriptorSets> descriptorSets;
        std::unique_ptr<CommandBuffers> commandBuffers;
        std::unique_ptr<SyncObjects> syncObjects;

        const size_t MAX_FRAMES_IN_FLIGHT = 2;

    private:
        void IInit();
        void IInit(VulkanSettings &s);
        void IShutdown();
        void IOnUpdate(float &time);
        void IOnWindowResize(WindowResizeEvent &e);
        void IChangeSettings(VulkanSettings &s);

        size_t CurrentFrame = 0;
        bool m_FramebufferResized = false;
        void RecreateSwapChain();
        void DrawFrame(float &time);
    };
}
