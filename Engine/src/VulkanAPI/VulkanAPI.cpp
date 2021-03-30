#include "VulkanAPI/VulkanAPI.h"

#include "VulkanAPI/Instance.h"
#include "VulkanAPI/Device.h"
#include "VulkanAPI/SwapChain.h"
#include "VulkanAPI/Descriptors.h"
#include "VulkanAPI/GraphicsPipeline.h"
#include "VulkanAPI/Framebuffers.h"
#include "VulkanAPI/Commands.h"
#include "VulkanAPI/Buffers.h"
#include "VulkanAPI/SyncObjects.h"

namespace eng
{
    Vulkan::Vulkan()
        : m_Vertices{
              {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
              {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
              {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
              {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}},
          m_Indices{0, 1, 2, 2, 3, 0}
    {
    }

    Vulkan::~Vulkan()
    {
    }

    void Vulkan::IShutdown()
    {
        delete this;
    }

    void Vulkan::IInit()
    {
        instance = new Instance;
        debugMessenger = new DebugMessenger;
        GLFW::CreateWindowSurface(instance->m_VkInstance, Vulkan::Get()->instance->m_VkSurface);
        physicalDevice = new PhysicalDevice;
        logicalDevice = new LogicalDevice;
        swapChain = new SwapChain;
        imageViews = new ImageViews;
        renderPass = new RenderPass;
        descriptorSetLayout = new DescriptorSetLayout;
        graphicsPipeline = new GraphicsPipeline;
        framebuffers = new Framebuffers;
        commandPool = new CommandPool;
        memory = new Memory;
        vertexBuffer = new VertexBuffer;
        indexBuffer = new IndexBuffer;
        uniformBuffer = new UniformBuffer;
        descriptorPool = new DescriptorPool;
        descriptorSets = new DescriptorSets;
        commandBuffers = new CommandBuffers;
        syncObjects = new SyncObjects;
    }

    void Vulkan::IInit(VulkanSettings &s)
    {
        Vulkan::Get()->settings = s;
        Vulkan::Get()->IInit();
    }

    void Vulkan::IOnUpdate(float &time)
    {
        DrawFrame(time);
        vkDeviceWaitIdle(logicalDevice->m_VkLogicalDevice);
    }

    void Vulkan::IOnWindowResize(WindowResizeEvent &e)
    {
    }

    bool Vulkan::AreValidationLayersEnabled()
    {
#ifdef NDEBUG
        return false;
#else
        return true;
#endif
    }

    bool Vulkan::CheckValidationLayerSupport(const std::vector<const char *> &validationLayers)
    {
        uint32_t layerCount;
        std::vector<VkLayerProperties> availableLayers;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        availableLayers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        bool validationLayersSupported = IsSubset(validationLayers, validationLayers.size(), availableLayers, layerCount);
        return validationLayersSupported;
    }

    void Vulkan::IChangeSettings(VulkanSettings &s)
    {
        Vulkan::Get()->settings = s;
    }

    void Vulkan::RecreateSwapChain()
    {
        vkDeviceWaitIdle(Vulkan::Get()->logicalDevice->m_VkLogicalDevice);
        Vulkan::Get()->commandBuffers->~CommandBuffers();
        Vulkan::Get()->descriptorSets->~DescriptorSets();
        Vulkan::Get()->descriptorPool->~DescriptorPool();
        Vulkan::Get()->uniformBuffer->~UniformBuffer();
        Vulkan::Get()->framebuffers->~Framebuffers();
        Vulkan::Get()->renderPass->~RenderPass();
        Vulkan::Get()->imageViews->~ImageViews();
        Vulkan::Get()->swapChain->~SwapChain();

        Vulkan::Get()->swapChain = new SwapChain;
        Vulkan::Get()->imageViews = new ImageViews;
        Vulkan::Get()->renderPass = new RenderPass;
        Vulkan::Get()->framebuffers = new Framebuffers;
        Vulkan::Get()->uniformBuffer = new UniformBuffer;
        Vulkan::Get()->descriptorPool = new DescriptorPool;
        Vulkan::Get()->descriptorSets = new DescriptorSets;
        Vulkan::Get()->commandBuffers = new CommandBuffers;
    }
}