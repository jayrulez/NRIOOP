/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include "VKDLL.h"

struct VkImageSubresourceRange;

#if _WIN32
struct IDXGIAdapter;
#endif

namespace nri
{
    NRI_VK_API  Result GetPhysicalDevices(PhysicalDeviceGroup* physicalDeviceGroups, uint32_t& physicalDeviceGroupNum);

    struct WrapperVKInterface
    {
        Result (*CreateCommandQueueVK)(Device& device, const CommandQueueVulkanDesc& commandQueueVulkanDesc, CommandQueue*& commandQueue);
        Result (*CreateCommandAllocatorVK)(Device& device, const CommandAllocatorVulkanDesc& commandAllocatorVulkanDesc, CommandAllocator*& commandAllocator);
        Result (*CreateCommandBufferVK)(Device& device, const CommandBufferVulkanDesc& commandBufferVulkanDesc, CommandBuffer*& commandBuffer);
        Result (*CreateDescriptorPoolVK)(Device& device, NRIVkDescriptorPool vkDescriptorPool, DescriptorPool*& descriptorPool);
        Result (*CreateBufferVK)(Device& device, const BufferVulkanDesc& bufferVulkanDesc, Buffer*& buffer);
        Result (*CreateTextureVK)(Device& device, const TextureVulkanDesc& textureVulkanDesc, Texture*& texture);
        Result (*CreateMemoryVK)(Device& device, const MemoryVulkanDesc& memoryVulkanDesc, Memory*& memory);
        Result (*CreateGraphicsPipelineVK)(Device& device, NRIVkPipeline vkPipeline, Pipeline*& pipeline);
        Result (*CreateComputePipelineVK)(Device& device, NRIVkPipeline vkPipeline, Pipeline*& pipeline);
        Result (*CreateQueryPoolVK)(Device& device, const QueryPoolVulkanDesc& queryPoolVulkanDesc, QueryPool*& queryPool);
        Result (*CreateQueueSemaphoreVK)(Device& device, NRIVkSemaphore vkSemaphore, QueueSemaphore*& queueSemaphore);
        Result (*CreateDeviceSemaphoreVK)(Device& device, NRIVkFence vkFence, DeviceSemaphore*& deviceSemaphore);

        NRIVkDevice (*GetDeviceVK)(const Device& device);
        NRIVkPhysicalDevice (*GetPhysicalDeviceVK)(const Device& device);
        NRIVkInstance (*GetInstanceVK)(const Device& device);
        NRIVkCommandBuffer (*GetCommandBufferVK)(const CommandBuffer& commandBuffer);

        void (*GetTextureVK)(const Texture& texture, uint32_t physicalDeviceIndex, TextureVulkanDesc& textureVulkanDesc);
        NRIVkImageView (*GetTextureDescriptorVK)(const Descriptor& descriptor, uint32_t physicalDeviceIndex, VkImageSubresourceRange& subresource);
        NRIVkBufferView (*GetBufferDescriptorVK)(const Descriptor& descriptor, uint32_t physicalDeviceIndex);
    };
}
