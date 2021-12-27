#pragma once

#include "BaseDLL.h"
#include "Base.h"

namespace nri
{
	class NRI_BASE_API Device
	{
	public:
		virtual  void SetDebugName(const char* name) = 0;
		virtual  const DeviceDesc& GetDesc() const = 0;
		virtual  Result GetCommandQueue(CommandQueueType commandQueueType, CommandQueue*& commandQueue) = 0;

		virtual  Result CreateCommandAllocator(const CommandQueue& commandQueue, uint32_t physicalDeviceMask, CommandAllocator*& commandAllocator) = 0;
		virtual   Result CreateDescriptorPool(const DescriptorPoolDesc& descriptorPoolDesc, DescriptorPool*& descriptorPool) = 0;
		virtual   Result CreateBuffer(const BufferDesc& bufferDesc, Buffer*& buffer) = 0;
		virtual   Result CreateTexture(const TextureDesc& textureDesc, Texture*& texture) = 0;
		virtual   Result CreateBufferView(const BufferViewDesc& bufferViewDesc, Descriptor*& bufferView) = 0;
		virtual   Result CreateTexture1DView(const Texture1DViewDesc& textureViewDesc, Descriptor*& textureView) = 0;
		virtual   Result CreateTexture2DView(const Texture2DViewDesc& textureViewDesc, Descriptor*& textureView) = 0;
		virtual   Result CreateTexture3DView(const Texture3DViewDesc& textureViewDesc, Descriptor*& textureView) = 0;
		virtual   Result CreateSampler(const SamplerDesc& samplerDesc, Descriptor*& sampler) = 0;
		virtual   Result CreatePipelineLayout(const PipelineLayoutDesc& pipelineLayoutDesc, PipelineLayout*& pipelineLayout) = 0;
		virtual    Result CreateGraphicsPipeline(const GraphicsPipelineDesc& graphicsPipelineDesc, Pipeline*& pipeline) = 0;
		virtual   Result CreateComputePipeline(const ComputePipelineDesc& computePipelineDesc, Pipeline*& pipeline) = 0;
		virtual   Result CreateFrameBuffer(const FrameBufferDesc& frameBufferDesc, FrameBuffer*& frameBuffer) = 0;
		virtual    Result CreateQueryPool(const QueryPoolDesc& queryPoolDesc, QueryPool*& queryPool) = 0;
		virtual    Result CreateQueueSemaphore(QueueSemaphore*& queueSemaphore) = 0;
		virtual   Result CreateDeviceSemaphore(bool signaled, DeviceSemaphore*& deviceSemaphore) = 0;
		virtual   Result CreateSwapChain(const SwapChainDesc& swapChainDesc, SwapChain*& swapChain) = 0;
		virtual   Result CreateRayTracingPipeline(const RayTracingPipelineDesc& rayTracingPipelineDesc, Pipeline*& pipeline) = 0;
		virtual  Result CreateAccelerationStructure(const AccelerationStructureDesc& accelerationStructureDesc, AccelerationStructure*& accelerationStructure) = 0;

		virtual   void DestroyCommandAllocator(CommandAllocator& commandAllocator) = 0;
		virtual   void DestroyCommandBuffer(CommandBuffer& commandBuffer) = 0;
		virtual   void DestroyDescriptorPool(DescriptorPool& descriptorPool) = 0;
		virtual   void DestroyBuffer(Buffer& buffer) = 0;
		virtual   void DestroyTexture(Texture& texture) = 0;
		virtual   void DestroyDescriptor(Descriptor& descriptor) = 0;
		virtual   void DestroyPipelineLayout(PipelineLayout& pipelineLayout) = 0;
		virtual   void DestroyPipeline(Pipeline& pipeline) = 0;
		virtual   void DestroyFrameBuffer(FrameBuffer& frameBuffer) = 0;
		virtual   void DestroyQueryPool(QueryPool& queryPool) = 0;
		virtual  void DestroyQueueSemaphore(QueueSemaphore& queueSemaphore) = 0;
		virtual  void DestroyDeviceSemaphore(DeviceSemaphore& deviceSemaphore) = 0;
		virtual   void DestroySwapChain(SwapChain& swapChain) = 0;
		virtual   void DestroyAccelerationStructure(AccelerationStructure& accelerationStructure) = 0;

		virtual   Result GetDisplays(Display** displays, uint32_t& displayNum) = 0;
		virtual   Result GetDisplaySize(Display& display, uint16_t& width, uint16_t& height) = 0;

		virtual   Result AllocateMemory(uint32_t physicalDeviceMask, MemoryType memoryType, uint64_t size, Memory*& memory) = 0;
		virtual   Result BindBufferMemory(const BufferMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) = 0;
		virtual   Result BindTextureMemory(const TextureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) = 0;
		virtual   Result BindAccelerationStructureMemory(const AccelerationStructureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) = 0;
		virtual  void FreeMemory(Memory& memory) = 0;

		virtual   FormatSupportBits GetFormatSupport(Format format) const = 0;

		virtual   uint32_t CalculateAllocationNumber(const ResourceGroupDesc& resourceGroupDesc) const = 0;
		virtual   Result AllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations) = 0;

	public:
		Device(const Logger& log, const StdAllocator<uint8_t>& stdAllocator);

		virtual ~Device() {}
		virtual void Destroy() = 0;

		const Logger& GetLog() const;
		StdAllocator<uint8_t>& GetStdAllocator();
	protected:
		Logger m_Log;
		StdAllocator<uint8_t> m_StdAllocator;
	};

	inline Device::Device(const Logger& log, const StdAllocator<uint8_t>& stdAllocator) :
		m_Log(log),
		m_StdAllocator(stdAllocator)
	{
	}

	inline const Logger& Device::GetLog() const
	{
		return m_Log;
	}

	inline StdAllocator<uint8_t>& Device::GetStdAllocator()
	{
		return m_StdAllocator;
	}
}