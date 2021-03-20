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

        GLFW::CreateWindowSurface(instance->m_VkInstance, surface);

        physicalDevice = std::make_unique<PhysicalDevice>();
        /*m_LogicalDevice = std::make_unique<LogicalDevice>();
        m_SwapChain = std::make_unique<SwapChain>();
        m_ImageViews = std::make_unique<ImageViews>();
        m_RenderPass = std::make_unique<RenderPass>();
        m_GraphicsPipeline = std::make_unique<GraphicsPipeline>();
        m_Framebuffers = std::make_unique<Framebuffers>();
        m_CommandPool = std::make_unique<CommandPool>();
        m_CommandBuffers = std::make_unique<CommandBuffers>();
        m_SyncObjects = std::make_unique<SyncObjects>(); */
    }

    void Vulkan::IOnUpdate()
    {
    }

    void Vulkan::IOnWindowResize(WindowResizeEvent &e)
    {
    }

    void Vulkan::IShutdown()
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
}