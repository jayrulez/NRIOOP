/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include "D3D12DLL.h"

#include "Device.h"

#include <dxgi1_5.h>

#include <d3d12.h>

namespace nri
{
    struct  NRI_D3D12_API DeviceD3D12 final : public Device
    {
        DeviceD3D12(const Logger& log, StdAllocator<uint8_t>& stdAllocator);
        ~DeviceD3D12();

        operator ID3D12Device*() const;
        operator ID3D12Device5*() const;

        template<typename Implementation, typename Interface, typename ... Args>
        Result CreateImplementation(Interface*& entity, const Args&... args);

        Result Create(IDXGIAdapter* dxgiAdapter, bool enableValidation);
        Result Create(const DeviceCreationD3D12Desc& deviceCreationDesc);

        Result CreateCpuOnlyVisibleDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorNum);
        Result GetDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE type, DescriptorHandle& descriptorHandle);
        DescriptorPointerCPU GetDescriptorPointerCPU(const DescriptorHandle& descriptorHandle) const;
        DescriptorPointerGPU GetDescriptorPointerGPU(const DescriptorHandle& descriptorHandle) const;
        void GetMemoryInfo(MemoryLocation memoryLocation, const D3D12_RESOURCE_DESC& resourceDesc, MemoryDesc& memoryDesc) const;

        ID3D12CommandSignature* CreateCommandSignature(D3D12_INDIRECT_ARGUMENT_TYPE indirectArgumentType, uint32_t stride);
        ID3D12CommandSignature* GetDrawCommandSignature(uint32_t stride);
        ID3D12CommandSignature* GetDrawIndexedCommandSignature(uint32_t stride);
        ID3D12CommandSignature* GetDispatchCommandSignature() const;

        bool IsMeshShaderSupported() const;

        bool GetOutput(Display* display, ComPtr<IDXGIOutput>& output) const;

        //================================================================================================================
        // NRI
        //================================================================================================================
        virtual void SetDebugName(const char* name) override;

        virtual const DeviceDesc& GetDesc() const override;
        virtual Result GetCommandQueue(CommandQueueType commandQueueType, CommandQueue*& commandQueue) override;

        Result CreateCommandQueue(CommandQueueType commandQueueType, CommandQueue*& commandQueue);
         Result CreateCommandQueue(void* d3d12commandQueue, CommandQueueD3D12*& commandQueue);

        virtual  Result CreateCommandAllocator(const CommandQueue& commandQueue, uint32_t physicalDeviceMask, CommandAllocator*& commandAllocator) override;
        virtual  Result CreateDescriptorPool(const DescriptorPoolDesc& descriptorPoolDesc, DescriptorPool*& descriptorPool) override;
        virtual  Result CreateBuffer(const BufferDesc& bufferDesc, Buffer*& buffer) override;
        virtual  Result CreateTexture(const TextureDesc& textureDesc, Texture*& texture) override;
        virtual  Result CreateBufferView(const BufferViewDesc& bufferViewDesc, Descriptor*& bufferView) override;
        virtual  Result CreateTexture1DView(const Texture1DViewDesc& textureViewDesc, Descriptor*& textureView) override;
        virtual   Result CreateTexture2DView(const Texture2DViewDesc& textureViewDesc, Descriptor*& textureView) override;
        virtual   Result CreateTexture3DView(const Texture3DViewDesc& textureViewDesc, Descriptor*& textureView) override;
#ifdef __ID3D12GraphicsCommandList4_INTERFACE_DEFINED__
         Result CreateDescriptor(const AccelerationStructure& accelerationStructure, Descriptor*& accelerationStructureView);
#endif
        virtual   Result CreateSampler(const SamplerDesc& samplerDesc, Descriptor*& sampler) override;
        virtual   Result CreatePipelineLayout(const PipelineLayoutDesc& pipelineLayoutDesc, PipelineLayout*& pipelineLayout) override;
        virtual   Result CreateGraphicsPipeline(const GraphicsPipelineDesc& graphicsPipelineDesc, Pipeline*& pipeline) override;
        virtual    Result CreateComputePipeline(const ComputePipelineDesc& computePipelineDesc, Pipeline*& pipeline) override;
        virtual    Result CreateRayTracingPipeline(const RayTracingPipelineDesc& rayTracingPipelineDesc, Pipeline*& pipeline) override;
        virtual    Result CreateFrameBuffer(const FrameBufferDesc& frameBufferDesc, FrameBuffer*& frameBuffer) override;
        virtual   Result CreateQueryPool(const QueryPoolDesc& queryPoolDesc, QueryPool*& queryPool) override;
        virtual   Result CreateQueueSemaphore(QueueSemaphore*& queueSemaphore) override;
        virtual   Result CreateDeviceSemaphore(bool signaled, DeviceSemaphore*& deviceSemaphore) override;

           Result CreateCommandBuffer(const CommandBufferD3D12Desc& commandBufferDesc, CommandBuffer*& commandBuffer) ;
           Result CreateBuffer(const BufferD3D12Desc& commandBufferDesc, Buffer*& commandBuffer) ;
           Result CreateTexture(const TextureD3D12Desc& commandBufferDesc, Texture*& commandBuffer) ;
           Result CreateMemory(const MemoryD3D12Desc& memoryDesc, Memory*& memory) ;

        virtual   void DestroyCommandAllocator(CommandAllocator& commandAllocator) override;
        virtual   void DestroyCommandBuffer(CommandBuffer& commandBuffer) override;
        virtual   void DestroyDescriptorPool(DescriptorPool& descriptorPool) override;
        virtual   void DestroyBuffer(Buffer& buffer) override;
        virtual   void DestroyTexture(Texture& texture) override;
        virtual   void DestroyDescriptor(Descriptor& descriptor) override;
        virtual   void DestroyPipelineLayout(PipelineLayout& pipelineLayout) override;
        virtual   void DestroyPipeline(Pipeline& pipeline) override;
        virtual   void DestroyFrameBuffer(FrameBuffer& frameBuffer) override;
        virtual   void DestroyQueryPool(QueryPool& queryPool) override;
        virtual   void DestroyQueueSemaphore(QueueSemaphore& queueSemaphore) override;
        virtual   void DestroyDeviceSemaphore(DeviceSemaphore& deviceSemaphore) override;
        virtual  void DestroySwapChain(SwapChain& swapChain) override;

        virtual  Result AllocateMemory(uint32_t physicalDeviceMask, const MemoryType memoryType, uint64_t size, Memory*& memory) override;
        virtual   Result BindBufferMemory(const BufferMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override;
        virtual   Result BindTextureMemory(const TextureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override;
#ifdef __ID3D12GraphicsCommandList4_INTERFACE_DEFINED__
        virtual   Result CreateAccelerationStructure(const AccelerationStructureDesc& accelerationStructureDesc, AccelerationStructure*& accelerationStructure) override;
        virtual  Result BindAccelerationStructureMemory(const AccelerationStructureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override;
        virtual  void DestroyAccelerationStructure(AccelerationStructure& accelerationStructure) override;
#endif
        virtual  void FreeMemory(Memory& memory) override;

        virtual  FormatSupportBits GetFormatSupport(Format format) const override;

        virtual   uint32_t CalculateAllocationNumber(const ResourceGroupDesc& resourceGroupDesc) const override;
        virtual   Result AllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations) override;

        virtual   Result CreateSwapChain(const SwapChainDesc& swapChainDesc, SwapChain*& swapChain) override;
        virtual   Result GetDisplays(Display** displays, uint32_t& displayNum) override;
        virtual     Result GetDisplaySize(Display& display, uint16_t& width, uint16_t& height) override;

        //================================================================================================================
        // DeviceBase
        //================================================================================================================
        virtual  void Destroy() override;

    private:
        void UpdateDeviceDesc(bool enableValidation);
        MemoryType GetMemoryType(MemoryLocation memoryLocation, const D3D12_RESOURCE_DESC& resourceDesc) const;

    private:
        ComPtr<ID3D12Device> m_Device;
#ifdef __ID3D12Device5_INTERFACE_DEFINED__
        ComPtr<ID3D12Device5> m_Device5;
#endif
        std::array<CommandQueueD3D12*, COMMAND_QUEUE_TYPE_NUM> m_CommandQueues = {};
        Vector<DescriptorHeapDesc> m_DescriptorHeaps;
        static const uint32_t m_DescriptorHeapTypeNum = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
        Vector<Vector<DescriptorHandle>> m_DescriptorPool;
        DeviceDesc m_DeviceDesc = {};
        UnorderedMap<uint32_t, ComPtr<ID3D12CommandSignature>> m_DrawCommandSignatures;
        UnorderedMap<uint32_t, ComPtr<ID3D12CommandSignature>> m_DrawIndexedCommandSignatures;
        ComPtr<ID3D12CommandSignature> m_DispatchCommandSignature;
        ComPtr<IDXGIAdapter> m_Adapter;
        bool m_IsRaytracingSupported = false;
        bool m_IsMeshShaderSupported = false;
        bool m_SkipLiveObjectsReporting = false;
    };

    inline DeviceD3D12::operator ID3D12Device*() const
    {
        return m_Device.GetInterface();
    }

#ifdef __ID3D12Device5_INTERFACE_DEFINED__
    inline DeviceD3D12::operator ID3D12Device5*() const
    {
        return m_Device5.GetInterface();
    }
#endif

    inline bool DeviceD3D12::IsMeshShaderSupported() const
    {
        return m_IsMeshShaderSupported;
    }
}

NRI_D3D12_API nri::Result CreateDeviceD3D12(const nri::DeviceCreationDesc& deviceCreationDesc, nri::Device*& device);
NRI_D3D12_API nri::Result CreateDeviceD3D12(const nri::DeviceCreationD3D12Desc& deviceCreationDesc, nri::Device*& device);
