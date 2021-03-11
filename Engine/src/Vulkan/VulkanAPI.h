#pragma once

#include <vulkan/vulkan.h>
#include "GLFW/GLFW.h"

namespace eng
{
    class VulkanAPI
    {
    public:
        VulkanAPI(GLFW &glfw);
        ~VulkanAPI();

    private:
        VkInstance instance;
        void CreateInstance(GLFW &glfw);

        template <typename S, typename A>
        bool IsSubset(S &subset, const uint16_t &subsetCount, A &array, const uint16_t &arrayCount);

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif

        const std::vector<const char *> validationLayers;

        void Cleanup();
    };
}