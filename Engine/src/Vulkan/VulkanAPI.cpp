#include "pch.h"
#include "Vulkan/VulkanAPI.h"

namespace eng
{
    VulkanAPI::VulkanAPI()
    {
        createInstance();
    }

    VulkanAPI::~VulkanAPI()
    {
    }
}