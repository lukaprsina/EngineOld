#include "Vulkan/VulkanAPI.h"

// TODO: add functionality
namespace eng
{
    void VulkanAPI::CreateSwapChain()
    {
        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(m_GPUProperties.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(m_GPUProperties.presentModes);
        VkExtent2D extent = ChooseSwapExtent(m_GPUProperties.capabilities);

        uint32_t imageCount = m_GPUProperties.capabilities.minImageCount + 1;

        if (m_GPUProperties.capabilities.maxImageCount > 0 && imageCount > m_GPUProperties.capabilities.maxImageCount)
        {
            imageCount = m_GPUProperties.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_Surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = {m_GPUProperties.graphicsFamily.value(), m_GPUProperties.presentFamily.value()};

        if (m_GPUProperties.graphicsFamily != m_GPUProperties.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = m_GPUProperties.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_FALSE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_LogicalDevice, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;
    }

    void VulkanAPI::QuerySwapChainSupport(DeviceInfo &indices)
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(indices.device, m_Surface, &indices.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(indices.device, m_Surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            indices.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(indices.device, m_Surface, &formatCount, indices.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(indices.device, m_Surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            indices.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(indices.device, m_Surface, &presentModeCount, indices.presentModes.data());
        }
    }

    VkSurfaceFormatKHR VulkanAPI::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
        for (const auto &availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanAPI::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
    {
        for (const auto &availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanAPI::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
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