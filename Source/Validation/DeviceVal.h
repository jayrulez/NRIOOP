/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include "ValidationDLL.h"
#include "Device.h"
#include "Lock.h"

namespace nri
{
    struct CommandQueueVal;

    struct NRI_VALIDATION_API DeviceVal final : public Device
    {
        DeviceVal(const Logger& log, const StdAllocator<uint8_t>& stdAllocator, Device& device, uint32_t physicalDeviceNum);
        virtual ~DeviceVal();
        virtual void Destroy() override;

        bool Create();
        void RegisterMemoryType(MemoryType memoryType, MemoryLocation memoryLocation);

        virtual void SetDebugName(const char* name) override;
        virtual const DeviceDesc& GetDesc() const override;
        virtual Result GetCommandQueue(CommandQueueType commandQueueType, CommandQueue*& commandQueue) override;

        virtual Result CreateCommandAllocator(const CommandQueue& commandQueue, uint32_t physicalDeviceMask, CommandAllocator*& commandAllocator) override;
        virtual Result CreateDescriptorPool(const DescriptorPoolDesc& descriptorPoolDesc, DescriptorPool*& descriptorPool) override;
        virtual Result CreateBuffer(const BufferDesc& bufferDesc, Buffer*& buffer) override;
        virtual Result CreateTexture(const TextureDesc& textureDesc, Texture*& texture) override;
        virtual Result CreateBufferView(const BufferViewDesc& bufferViewDesc, Descriptor*& bufferView) override;
        virtual Result CreateTexture1DView(const Texture1DViewDesc& textureViewDesc, Descriptor*& textureView) override;
        virtual Result CreateTexture2DView(const Texture2DViewDesc& textureViewDesc, Descriptor*& textureView) override;
        virtual Result CreateTexture3DView(const Texture3DViewDesc& textureViewDesc, Descriptor*& textureView) override;
        virtual Result CreateSampler(const SamplerDesc& samplerDesc, Descriptor*& sampler) override;
        virtual Result CreatePipelineLayout(const PipelineLayoutDesc& pipelineLayoutDesc, PipelineLayout*& pipelineLayout) override;
        virtual Result CreateGraphicsPipeline(const GraphicsPipelineDesc& graphicsPipelineDesc, Pipeline*& pipeline) override;
        virtual Result CreateComputePipeline(const ComputePipelineDesc& computePipelineDesc, Pipeline*& pipeline) override;
        virtual Result CreateFrameBuffer(const FrameBufferDesc& frameBufferDesc, FrameBuffer*& frameBuffer) override;
        virtual Result CreateQueryPool(const QueryPoolDesc& queryPoolDesc, QueryPool*& queryPool) override;
        virtual Result CreateQueueSemaphore(QueueSemaphore*& queueSemaphore) override;
        virtual Result CreateDeviceSemaphore(bool signaled, DeviceSemaphore*& deviceSemaphore) override;
        virtual  Result CreateSwapChain(const SwapChainDesc& swapChainDesc, SwapChain*& swapChain) override;
        virtual  Result CreateRayTracingPipeline(const RayTracingPipelineDesc& pipelineDesc, Pipeline*& pipeline) override;
        virtual  Result CreateAccelerationStructure(const AccelerationStructureDesc& accelerationStructureDesc, AccelerationStructure*& accelerationStructure) override;

        virtual void DestroyCommandAllocator(CommandAllocator& commandAllocator) override;
        virtual void DestroyCommandBuffer(CommandBuffer& commandBuffer) override;
        virtual void DestroyDescriptorPool(DescriptorPool& descriptorPool) override;
        virtual void DestroyBuffer(Buffer& buffer) override;
        virtual void DestroyTexture(Texture& texture) override;
        virtual void DestroyDescriptor(Descriptor& descriptor) override;
        virtual void DestroyPipelineLayout(PipelineLayout& pipelineLayout) override;
        virtual void DestroyPipeline(Pipeline& pipeline) override;
        virtual void DestroyFrameBuffer(FrameBuffer& frameBuffer) override;
        virtual void DestroyQueryPool(QueryPool& queryPool) override;
        virtual void DestroyQueueSemaphore(QueueSemaphore& queueSemaphore) override;
        virtual void DestroyDeviceSemaphore(DeviceSemaphore& deviceSemaphore) override;
        virtual  void DestroySwapChain(SwapChain& swapChain) override;
        virtual  void DestroyAccelerationStructure(AccelerationStructure& accelerationStructure) override;

        virtual  Result GetDisplays(Display** displays, uint32_t& displayNum) override;
        virtual  Result GetDisplaySize(Display& display, uint16_t& width, uint16_t& height) override;

        virtual Result AllocateMemory(uint32_t physicalDeviceMask, MemoryType memoryType, uint64_t size, Memory*& memory) override;
        virtual  Result BindBufferMemory(const BufferMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override;
        virtual  Result BindTextureMemory(const TextureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override;
        virtual   Result BindAccelerationStructureMemory(const AccelerationStructureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override;
        virtual  void FreeMemory(Memory& memory) override;

        virtual  FormatSupportBits GetFormatSupport(Format format) const override;


        virtual  uint32_t CalculateAllocationNumber(const ResourceGroupDesc& resourceGroupDesc) const override;
        virtual  Result AllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations) override;

        //
        uint32_t GetPhysicalDeviceNum() const;
        bool IsPhysicalDeviceMaskValid(uint32_t physicalDeviceMask) const;

        Lock& GetLock();

    private:
        Device& m_Device;
        String m_Name;
        bool m_IsSwapChainSupported = false;
        bool m_IsWrapperD3D11Supported = false;
        bool m_IsWrapperD3D12Supported = false;
        bool m_IsWrapperVKSupported = false;
        bool m_IsRayTracingSupported = false;
        bool m_IsMeshShaderExtSupported = false;
        uint32_t m_PhysicalDeviceNum = 0;
        uint32_t m_PhysicalDeviceMask = 0;
        std::array<CommandQueueVal*, COMMAND_QUEUE_TYPE_NUM> m_CommandQueues = {};
        UnorderedMap<MemoryType, MemoryLocation> m_MemoryTypeMap;
        Lock m_Lock;
    };

    inline uint32_t DeviceVal::GetPhysicalDeviceNum() const
    {
        return m_PhysicalDeviceNum;
    }

    inline bool DeviceVal::IsPhysicalDeviceMaskValid(uint32_t physicalDeviceMask) const
    {
        return (physicalDeviceMask & m_PhysicalDeviceMask) == physicalDeviceMask;
    }

    inline Lock& DeviceVal::GetLock()
    {
        return m_Lock;
    }
}

NRI_VALIDATION_API nri::Device* CreateDeviceValidation(const nri::DeviceCreationDesc& deviceCreationDesc, nri::Device& device);
