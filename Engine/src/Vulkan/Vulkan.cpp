#include "Vulkan/Vulkan.h"

#include "Vulkan/Instance.h"
#include "Vulkan/Device.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/ImageViews.h"
#include "Vulkan/RenderPass.h"
#include "Vulkan/GraphicsPipeline.h"
#include "Vulkan/Framebuffers.h"
#include "Vulkan/Commands.h"
#include "Vulkan/Buffers.h"
#include "Vulkan/SyncObjects.h"

namespace eng
{
    Vulkan::Vulkan()
        : /* m_Vertices{
              {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
              {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
              {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
              {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}},
          m_Indices{0, 1, 2, 2, 3, 0} */
          m_Vertices{
              {{-0.5f, -0.005f}, {1.0f, 0.0f, 0.0f}},
              {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
              {{0.5f, -0.495f}, {0.0f, 0.0f, 1.0f}},
              {{-0.5f, -0.0f}, {1.0f, 1.0f, 1.0f}}},
          m_Indices{0, 1, 2, 2, 3, 0}
    {
    }

    Vulkan::~Vulkan()
    {
    }

    void Vulkan::IInit()
    {
        instance = std::make_unique<Instance>();
        debugMessenger = std::make_unique<DebugMessenger>();

        GLFW::CreateWindowSurface(instance->m_VkInstance, Vulkan::Get().instance->m_VkSurface);

        physicalDevice = std::make_unique<PhysicalDevice>();
        logicalDevice = std::make_unique<LogicalDevice>();

        swapChain = std::make_unique<SwapChain>();
        imageViews = std::make_unique<ImageViews>();
        renderPass = std::make_unique<RenderPass>();
        graphicsPipeline = std::make_unique<GraphicsPipeline>();
        framebuffers = std::make_unique<Framebuffers>();

        commandPool = std::make_unique<CommandPool>();
        memory = std::make_unique<Memory>();
        vertexBuffer = std::make_unique<VertexBuffer>();
        indexBuffer = std::make_unique<IndexBuffer>();
        commandBuffers = std::make_unique<CommandBuffers>();
        syncObjects = std::make_unique<SyncObjects>();
    }

    void Vulkan::IInit(VulkanSettings &s)
    {
        Vulkan::Get().settings = s;
        Vulkan::Init();
    }

    void Vulkan::IOnUpdate()
    {
        DrawFrame();
        vkDeviceWaitIdle(logicalDevice->m_VkLogicalDevice);
    }

    void Vulkan::IOnWindowResize(WindowResizeEvent &e)
    {
    }

    void Vulkan::IShutdown()
    {
        Vulkan::Get().~Vulkan();
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
        Vulkan::Get().settings = s;
    }

    void Vulkan::RecreateSwapChain()
    {
        vkDeviceWaitIdle(Vulkan::Get().logicalDevice->m_VkLogicalDevice);
        Vulkan::Get().swapChain.reset();
        Vulkan::Get().imageViews.reset();
        Vulkan::Get().renderPass.reset();
        Vulkan::Get().framebuffers.reset();
        Vulkan::Get().commandBuffers.reset();

        Vulkan::Get().swapChain = std::make_unique<SwapChain>();
        Vulkan::Get().imageViews = std::make_unique<ImageViews>();
        Vulkan::Get().renderPass = std::make_unique<RenderPass>();
        Vulkan::Get().framebuffers = std::make_unique<Framebuffers>();
        Vulkan::Get().commandBuffers = std::make_unique<CommandBuffers>();
    }
}