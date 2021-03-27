#include "Vulkan/Vulkan.h"
#include "Vulkan/Device.h"
#include "Vulkan/Buffers.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/SyncObjects.h"
#include "Vulkan/Commands.h"

#define MAX_FRAMES_IN_FLIGHT 2

namespace eng
{
    void Vulkan::DrawFrame(float &time)
    {
        auto logicalDevice = Vulkan::Get().logicalDevice->m_VkLogicalDevice;
        auto swapChain = Vulkan::Get().swapChain->m_VkSwapChain;
        auto inFlightFences = Vulkan::Get().syncObjects->m_VkInFlightFences;
        auto imageAvailableSemaphores = Vulkan::Get().syncObjects->m_VkImageAvailableSemaphores;
        auto imagesInFlight = Vulkan::Get().syncObjects->m_VkImagesInFlight;

        vkWaitForFences(logicalDevice, 1,
                        &inFlightFences[CurrentFrame],
                        VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX,
                                                imageAvailableSemaphores[CurrentFrame],
                                                VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            Vulkan::RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
        {
            vkWaitForFences(logicalDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }

        imagesInFlight[imageIndex] = inFlightFences[CurrentFrame];

        Vulkan::Get().uniformBuffer->OnUpdate(imageIndex, time);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[CurrentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &Vulkan::Get().commandBuffers->m_VkCommandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {Vulkan::Get().syncObjects->m_VkRenderFinishedSemaphores[CurrentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(logicalDevice, 1, &inFlightFences[CurrentFrame]);

        if (vkQueueSubmit(Vulkan::Get().logicalDevice->m_VkGraphicsQueue, 1, &submitInfo, inFlightFences[CurrentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(Vulkan::Get().logicalDevice->m_VkPresentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized)
        {
            m_FramebufferResized = false;
            Vulkan::Get().RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        CurrentFrame = (CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        /* auto timelineSemaphore = Vulkan::Get().syncObjects->m_VkTimeline;
        auto commandBuffers = Vulkan::Get().commandBuffers->m_VkCommandBuffers;

        const uint64_t waitValue1 = 0;
        const uint64_t signalValue1 = 1;

        VkTimelineSemaphoreSubmitInfo timelineInfo{};
        timelineInfo.sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO;
        timelineInfo.waitSemaphoreValueCount = 1;
        timelineInfo.pWaitSemaphoreValues = &waitValue1;
        timelineInfo.signalSemaphoreValueCount = 1;
        timelineInfo.pSignalSemaphoreValues = &signalValue1;

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(Vulkan::Get().logicalDevice->m_VkLogicalDevice,
                                                Vulkan::Get().swapChain->m_VkSwapChain,
                                                UINT64_MAX,
                                                timelineSemaphore,
                                                VK_NULL_HANDLE,
                                                &imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = &timelineInfo;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &timelineSemaphore;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &timelineSemaphore;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        if (vkQueueSubmit(Vulkan::Get().logicalDevice->m_VkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        const uint64_t waitValue2 = 1;

        VkSemaphoreWaitInfo waitInfo;
        waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
        waitInfo.pNext = NULL;
        waitInfo.flags = 0;
        waitInfo.semaphoreCount = 1;
        waitInfo.pSemaphores = &timelineSemaphore;
        waitInfo.pValues = &waitValue2;

        vkWaitSemaphores(Vulkan::Get().logicalDevice->m_VkLogicalDevice, &waitInfo, UINT64_MAX); */
    }
}
