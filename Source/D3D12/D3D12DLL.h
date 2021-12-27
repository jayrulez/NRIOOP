#pragma once

#include "BaseDLL.h"
#include "Base.h"

#if _WIN32
#if defined(_MSC_VER)
#if defined(NRI_STATIC_LIBRARY)
#define NRI_D3D12_API
#else
#if (NRI_D3D12_EXPORTS == 1)
#define NRI_D3D12_API __declspec(dllexport)
#else
#define NRI_D3D12_API __declspec(dllexport)
#endif
#endif
#else
#if defined(NRI_STATIC_LIBRARY)
#define NRI_D3D12_API
#else
#if (NRI_D3D12_EXPORTS == 1)
#define NRI_D3D12_API __attribute__ ((dllexport))
#else
#define NRI_D3D12_API __attribute__ ((dllimport))
#endif
#endif
#endif
#else
#define NRI_D3D12_API __attribute__ ((visibility ("default")))
#endif

struct ID3D12Device;
struct ID3D12CommandQueue;
struct ID3D12Resource;
struct ID3D12Heap;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandAllocator;
struct IDXGIAdapter;
struct IDXGIFactory2;
struct ID3D12Device5;
struct ID3D12DescriptorHeap;
struct ID3D12CommandSignature;
struct IDXGIOutput;
struct D3D12_CPU_DESCRIPTOR_HANDLE;

#define DESCRIPTORS_BATCH_SIZE          1024

typedef size_t DescriptorPointerCPU;
typedef uint64_t DescriptorPointerGPU;
typedef uint16_t HeapIndexType;
typedef uint16_t HeapOffsetType;

namespace nri
{
    struct DeviceD3D12;
    struct BufferD3D12;
    struct MemoryD3D12;
    struct TextureD3D12;
    struct PipelineLayoutD3D12;
    struct CommandBufferD3D12;
    struct DescriptorSetD3D12;
    struct PipelineD3D12;
    struct DescriptorPoolD3D12;
    struct FrameBufferD3D12;
    struct CommandQueueD3D12;

    struct NRI_D3D12_API DescriptorHandle
    {
        HeapIndexType heapIndex;
        HeapOffsetType heapOffset;
    };

    struct NRI_D3D12_API DescriptorHeapDesc
    {
        ComPtr<ID3D12DescriptorHeap> descriptorHeap;
        DescriptorPointerCPU descriptorPointerCPU;
        DescriptorPointerGPU descriptorPointerGPU;
        uint32_t descriptorSize;
    };

    struct NRI_D3D12_API DeviceCreationD3D12Desc
    {
        ID3D12Device* d3d12Device;
        ID3D12CommandQueue* d3d12GraphicsQueue;
        ID3D12CommandQueue* d3d12ComputeQueue;
        ID3D12CommandQueue* d3d12CopyQueue;
        IDXGIAdapter* d3d12PhysicalAdapter;
        CallbackInterface callbackInterface;
        MemoryAllocatorInterface memoryAllocatorInterface;
        bool enableNRIValidation;
        bool enableAPIValidation;
    };

    struct NRI_D3D12_API CommandBufferD3D12Desc
    {
        ID3D12GraphicsCommandList* d3d12CommandList;
        ID3D12CommandAllocator* d3d12CommandAllocator;
    };

    struct NRI_D3D12_API BufferD3D12Desc
    {
        ID3D12Resource* d3d12Resource;
        uint32_t structureStride;
    };

    struct NRI_D3D12_API TextureD3D12Desc
    {
        ID3D12Resource* d3d12Resource;
    };

    struct NRI_D3D12_API MemoryD3D12Desc
    {
        ID3D12Heap* d3d12Heap;
    };

    struct WrapperD3D12Interface
    {
        Result(*CreateCommandBufferD3D12)(Device& device, const CommandBufferD3D12Desc& commandBufferDesc, CommandBuffer*& commandBuffer);
        Result(*CreateBufferD3D12)(Device& device, const BufferD3D12Desc& bufferDesc, Buffer*& buffer);
        Result(*CreateTextureD3D12)(Device& device, const TextureD3D12Desc& textureDesc, Texture*& texture);
        Result(*CreateMemoryD3D12)(Device& device, const MemoryD3D12Desc& memoryDesc, Memory*& memory);

        ID3D12Device* (*GetDeviceD3D12)(const Device& device);
        ID3D12Resource* (*GetBufferD3D12)(const Buffer& buffer);
        ID3D12Resource* (*GetTextureD3D12)(const Texture& texture);
        ID3D12GraphicsCommandList* (*GetCommandBufferD3D12)(const CommandBuffer& commandBuffer);
    };
}