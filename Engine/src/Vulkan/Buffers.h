#pragma once

#include "Vulkan/Vulkan.h"

namespace eng
{
    class Memory
    {
    public:
        Memory();
        ~Memory();
        VmaAllocator m_VmaMemory;
        static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    };

    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        VkBuffer m_VkVertexBuffer;
        VmaAllocation m_VmaVertexAllocation;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();

        VkBuffer m_VkIndexBuffer;
        VmaAllocation m_VmaIndexAllocation;
    };
}