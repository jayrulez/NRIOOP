#pragma once

#include "BaseDLL.h"
#include "Base.h"

#include <vulkan/vulkan.h>

#if _WIN32
#if defined(_MSC_VER)
#if defined(NRI_STATIC_LIBRARY)
#define NRI_VK_API
#else
#if (NRI_VK_EXPORTS == 1)
#define NRI_VK_API __declspec(dllexport)
#else
#define NRI_VK_API __declspec(dllexport)
#endif
#endif
#else
#if defined(NRI_STATIC_LIBRARY)
#define NRI_VK_API
#else
#if (NRI_VK_EXPORTS == 1)
#define NRI_VK_API __attribute__ ((dllexport))
#else
#define NRI_VK_API __attribute__ ((dllimport))
#endif
#endif
#endif
#else
#define NRI_VK_API __attribute__ ((visibility ("default")))
#endif

namespace nri 
{
    struct DeviceVK;
    struct MemoryVK;
    struct BufferVK;
    struct PipelineVK;
    struct PipelineLayoutVK;
    struct TextureVK;
    struct FrameBufferVK;
    struct DescriptorSetVK;
    struct CommandQueueVK;
    struct DescriptorVK;

    typedef uint64_t NRIVkInstance;
    typedef uint64_t NRIVkPhysicalDevice;
    typedef uint64_t NRIVkDevice;
    typedef uint64_t NRIVkQueue;
    typedef void* NRIVkCommandPool;
    typedef uint64_t NRIVkCommandBuffer;
    typedef void* NRIVkImage;
    typedef void* NRIVkBuffer;
    typedef void* NRIVkDeviceMemory;
    typedef void* NRIVkQueryPool;
    typedef void* NRIVkPipeline;
    typedef void* NRIVkDescriptorPool;
    typedef void* NRIVkSemaphore;
    typedef void* NRIVkFence;

    typedef void* NRIVkImageView;
    typedef void* NRIVkBufferView;

    struct NRI_VK_API DeviceCreationVulkanDesc
    {
        NRIVkInstance vkInstance;
        NRIVkDevice vkDevice;
        const NRIVkPhysicalDevice* vkPhysicalDevices;
        uint32_t deviceGroupSize;
        const uint32_t* queueFamilyIndices;
        uint32_t queueFamilyIndexNum;
        CallbackInterface callbackInterface;
        MemoryAllocatorInterface memoryAllocatorInterface;
        bool enableNRIValidation;
        bool enableAPIValidation;
        const char* const* instanceExtensions;
        uint32_t instanceExtensionNum;
        const char* const* deviceExtensions;
        uint32_t deviceExtensionNum;
        const char* vulkanLoaderPath;
    };

    struct NRI_VK_API CommandQueueVulkanDesc
    {
        NRIVkQueue vkQueue;
        uint32_t familyIndex;
        CommandQueueType commandQueueType;
    };

    struct NRI_VK_API CommandAllocatorVulkanDesc
    {
        NRIVkCommandPool vkCommandPool;
        CommandQueueType commandQueueType;
    };

    struct NRI_VK_API CommandBufferVulkanDesc
    {
        NRIVkCommandBuffer vkCommandBuffer;
        CommandQueueType commandQueueType;
    };

    struct NRI_VK_API BufferVulkanDesc
    {
        NRIVkBuffer vkBuffer;
        uint64_t bufferSize;
        Memory* memory;
        uint64_t memoryOffset;
        uint64_t deviceAddress;
        uint32_t physicalDeviceMask;
    };

    struct NRI_VK_API TextureVulkanDesc
    {
        NRIVkImage vkImage;
        uint32_t vkFormat;
        uint32_t vkImageAspectFlags;
        uint32_t vkImageType;
        uint16_t size[3];
        uint16_t mipNum;
        uint16_t arraySize;
        uint8_t sampleNum;
        uint32_t physicalDeviceMask;
    };

    struct NRI_VK_API MemoryVulkanDesc
    {
        NRIVkDeviceMemory vkDeviceMemory;
        uint64_t size;
        uint32_t memoryTypeIndex;
        uint32_t physicalDeviceMask;
    };

    struct NRI_VK_API QueryPoolVulkanDesc
    {
        NRIVkQueryPool vkQueryPool;
        uint32_t vkQueryType;
        uint32_t physicalDeviceMask;
    };
}