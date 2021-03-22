#include "Vulkan/Vulkan.h"

#include "Vulkan/Instance.h"
#include "Vulkan/DebugMessenger.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/LogicalDevice.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/ImageViews.h"
#include "Vulkan/RenderPass.h"
#include "Vulkan/GraphicsPipeline.h"
#include "Vulkan/Framebuffers.h"
#include "Vulkan/CommandPool.h"
#include "Vulkan/CommandBuffers.h"
#include "Vulkan/SyncObjects.h"

namespace eng
{
    Vulkan::Vulkan()
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
}