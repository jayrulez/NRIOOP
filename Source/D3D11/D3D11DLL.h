#pragma once

#include "BaseDLL.h"

#if _WIN32
#if defined(_MSC_VER)
#if defined(NRI_STATIC_LIBRARY)
#define NRI_D3D11_API
#else
#if (NRI_D3D11_EXPORTS == 1)
#define NRI_D3D11_API __declspec(dllexport)
#else
#define NRI_D3D11_API __declspec(dllexport)
#endif
#endif
#else
#if defined(NRI_STATIC_LIBRARY)
#define NRI_D3D11_API
#else
#if (NRI_D3D11_EXPORTS == 1)
#define NRI_D3D11_API __attribute__ ((dllexport))
#else
#define NRI_D3D11_API __attribute__ ((dllimport))
#endif
#endif
#endif
#else
#define NRI_D3D11_API __attribute__ ((visibility ("default")))
#endif

struct ID3D11Device;
struct ID3D11Resource;
struct ID3D11DeviceContext;

namespace nri {
    struct BufferD3D11;
    struct CommandQueueD3D11;
    struct CommandBufferD3D11;
    struct CommandAllocatorD3D11;
    struct DeviceD3D11;
    struct DescriptorD3D11;
    struct DescriptorPoolD3D11;
    struct DescriptorSetD3D11;
    struct DeviceSemaphoreD3D11;
    struct FrameBufferD3D11;
    struct MemoryD3D11;
    struct PipelineLayoutD3D11;
    struct PipelineD3D11;
    struct QueueSemaphoreD3D11;
    struct QueryPoolD3D11;
    struct TextureD3D11;

    struct NRI_D3D11_API DeviceCreationD3D11Desc
    {
        ID3D11Device* d3d11Device;
        void* agsContextAssociatedWithDevice;
        CallbackInterface callbackInterface;
        MemoryAllocatorInterface memoryAllocatorInterface;
        bool enableNRIValidation;
        bool enableAPIValidation;
    };

    struct NRI_D3D11_API CommandBufferD3D11Desc
    {
        ID3D11DeviceContext* d3d11DeviceContext;
    };

    struct NRI_D3D11_API BufferD3D11Desc
    {
        ID3D11Resource* d3d11Resource;
    };

    struct NRI_D3D11_API TextureD3D11Desc
    {
        ID3D11Resource* d3d11Resource;
    };

    struct WrapperD3D11Interface
    {
        Result(*CreateCommandBufferD3D11)(Device& device, const CommandBufferD3D11Desc& commandBufferDesc, CommandBuffer*& commandBuffer);
        Result(*CreateBufferD3D11)(Device& device, const BufferD3D11Desc& bufferDesc, Buffer*& buffer);
        Result(*CreateTextureD3D11)(Device& device, const TextureD3D11Desc& textureDesc, Texture*& texture);

        ID3D11Device* (*GetDeviceD3D11)(const Device& device);
        ID3D11Resource* (*GetBufferD3D11)(const Buffer& buffer);
        ID3D11Resource* (*GetTextureD3D11)(const Texture& texture);
        ID3D11DeviceContext* (*GetCommandBufferD3D11)(const CommandBuffer& commandBuffer);
    };
}