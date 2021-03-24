#include "Vulkan/Buffers.h"
#include "Vulkan/Instance.h"
#include "Vulkan/Device.h"
#include "Vulkan/Commands.h"

namespace eng
{
    Memory::Memory()
    {
        auto logicalDevice = Vulkan::Get().logicalDevice->m_VkLogicalDevice;
        auto vertices = Vulkan::Get().m_Vertices;

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
        allocatorInfo.physicalDevice = Vulkan::Get().physicalDevice->GPUProperties.device;
        allocatorInfo.device = logicalDevice;
        allocatorInfo.instance = Vulkan::Get().instance->m_VkInstance;

        vmaCreateAllocator(&allocatorInfo, &m_VmaMemory);
    }

    Memory::~Memory()
    {
        vmaDestroyAllocator(m_VmaMemory);
    }

    void Memory::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        auto graphicsQueue = Vulkan::Get().logicalDevice->m_VkGraphicsQueue;
        auto logicalDevice = Vulkan::Get().logicalDevice->m_VkLogicalDevice;

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = Vulkan::Get().commandPool->m_VkCommandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        vkFreeCommandBuffers(logicalDevice, Vulkan::Get().commandPool->m_VkCommandPool, 1, &commandBuffer);
    }

    VertexBuffer::VertexBuffer()
    {
        auto memory = Vulkan::Get().memory->m_VmaMemory;
        auto vertices = Vulkan::Get().m_Vertices;
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VmaAllocation stagingAllocation;

        VmaAllocationCreateInfo stagingAllocInfo{};
        stagingAllocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
        stagingAllocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VkBufferCreateInfo stagingBufferInfo{};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.size = bufferSize;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vmaCreateBuffer(memory, &stagingBufferInfo, &stagingAllocInfo,
                        &stagingBuffer, &stagingAllocation, nullptr);

        void *data;
        vmaMapMemory(memory, stagingAllocation, &data);
        memcpy(data, vertices.data(), stagingBufferInfo.size);
        vmaUnmapMemory(memory, stagingAllocation);

        VmaAllocationCreateInfo vertexAllocInfo{};
        vertexAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        vertexAllocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        VkBufferCreateInfo vertexBufferInfo{};
        vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vertexBufferInfo.size = bufferSize;
        vertexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vmaCreateBuffer(memory, &vertexBufferInfo, &vertexAllocInfo,
                        &m_VkVertexBuffer, &m_VmaVertexAllocation, nullptr);

        Memory::CopyBuffer(stagingBuffer, m_VkVertexBuffer, bufferSize);

        vmaDestroyBuffer(memory, stagingBuffer, stagingAllocation);
    }

    VertexBuffer::~VertexBuffer()
    {
        vmaDestroyBuffer(Vulkan::Get().memory->m_VmaMemory, m_VkVertexBuffer, m_VmaVertexAllocation);
    }

    IndexBuffer::IndexBuffer()
    {
        auto memory = Vulkan::Get().memory->m_VmaMemory;
        auto indices = Vulkan::Get().m_Indices;
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VmaAllocation stagingAllocation;

        VmaAllocationCreateInfo stagingAllocInfo{};
        stagingAllocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
        stagingAllocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VkBufferCreateInfo stagingBufferInfo{};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.size = bufferSize;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vmaCreateBuffer(memory, &stagingBufferInfo, &stagingAllocInfo,
                        &stagingBuffer, &stagingAllocation, nullptr);

        void *data;
        vmaMapMemory(memory, stagingAllocation, &data);
        memcpy(data, indices.data(), stagingBufferInfo.size);
        vmaUnmapMemory(memory, stagingAllocation);

        VmaAllocationCreateInfo vertexAllocInfo{};
        vertexAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        vertexAllocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        VkBufferCreateInfo vertexBufferInfo{};
        vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vertexBufferInfo.size = bufferSize;
        vertexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vmaCreateBuffer(memory, &vertexBufferInfo, &vertexAllocInfo,
                        &m_VkIndexBuffer, &m_VmaIndexAllocation, nullptr);

        Memory::CopyBuffer(stagingBuffer, m_VkIndexBuffer, bufferSize);

        vmaDestroyBuffer(memory, stagingBuffer, stagingAllocation);
    }

    IndexBuffer::~IndexBuffer()
    {
        vmaDestroyBuffer(Vulkan::Get().memory->m_VmaMemory, m_VkIndexBuffer, m_VmaIndexAllocation);
    }
}
