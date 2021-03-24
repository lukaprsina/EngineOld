#include "Vulkan/SwapChain.h"
#include "Vulkan/Instance.h"
#include "Vulkan/Device.h"

namespace eng
{
    SwapChain::SwapChain()
    {
        auto GPUProperties = Vulkan::Get().physicalDevice->GPUProperties;
        auto logicalDevice = Vulkan::Get().logicalDevice->m_VkLogicalDevice;

        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(GPUProperties.device);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = Vulkan::Get().instance->m_VkSurface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = {GPUProperties.graphicsFamily.value(), GPUProperties.presentFamily.value()};

        if (GPUProperties.graphicsFamily != GPUProperties.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_FALSE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &m_VkSwapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(logicalDevice, m_VkSwapChain, &imageCount, nullptr);
        m_VkSwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice, m_VkSwapChain, &imageCount, m_VkSwapChainImages.data());

        m_VkSwapChainImageFormat = surfaceFormat.format;
        m_VkSwapChainExtent = extent;
    }

    SwapChain::~SwapChain()
    {
        vkDestroySwapchainKHR(Vulkan::Get().logicalDevice->m_VkLogicalDevice, m_VkSwapChain, nullptr);
    }

    SwapChainSupportDetails SwapChain::QuerySwapChainSupport(VkPhysicalDevice &device)
    {
        auto surface = Vulkan::Get().instance->m_VkSurface;
        SwapChainSupportDetails swapChainSupport;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainSupport.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            swapChainSupport.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainSupport.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            swapChainSupport.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, swapChainSupport.presentModes.data());
        }

        return swapChainSupport;
    }

    VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
        auto settings = Vulkan::Get().settings;

        if (settings.SwapSurfaceFormat.has_value())
        {
            for (const auto &availableFormat : availableFormats)
            {
                if (availableFormat.format == settings.SwapSurfaceFormat.value().format &&
                    availableFormat.colorSpace == settings.SwapSurfaceFormat.value().colorSpace)
                {
                    return availableFormat;
                }
            }
        }

        for (const auto &availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
    {
        auto settings = Vulkan::Get().settings;

        if (settings.SwapSurfacePresentMode.has_value())
        {
            for (const auto &availablePresentMode : availablePresentModes)
            {
                if (availablePresentMode == settings.SwapSurfacePresentMode.value())
                {
                    return availablePresentMode;
                }
            }
        }

        for (const auto &availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        for (const auto &availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            GLFW::GetFramebufferSize(width, height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)};

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

}