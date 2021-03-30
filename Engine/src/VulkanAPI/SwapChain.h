#pragma once

#include "VulkanAPI/VulkanAPI.h"

namespace eng
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class SwapChain
    {
    public:
        SwapChain();
        ~SwapChain();

        VkSwapchainKHR m_VkSwapChain;
        VkFormat m_VkSwapChainImageFormat;
        VkExtent2D m_VkSwapChainExtent;
        std::vector<VkImage> m_VkSwapChainImages;

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice &device);
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    };
}