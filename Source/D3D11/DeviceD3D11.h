/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once
#include "D3D11DLL.h"
#include "SharedD3D11.h"
#include "D3D11Extensions.h"
#include "Device.h"
#include "Helpers.h"

#include "CommandQueueD3D11.h"

struct IDXGIOutput;

namespace nri
{
	struct NRI_D3D11_API DeviceD3D11 final : public Device
	{
		DeviceD3D11(const Logger& log, StdAllocator<uint8_t>& stdAllocator);
		~DeviceD3D11();

		const VersionedContext& GetImmediateContext() const;
		const VersionedDevice& GetDevice() const;
		bool GetOutput(Display* display, ComPtr<IDXGIOutput>& output) const;

		Result Create(const DeviceCreationDesc& deviceCreationDesc, IDXGIAdapter* adapter, ID3D11Device* precreatedDevice, AGSContext* agsContext);

		//======================================================================================================================
		// NRI
		//======================================================================================================================
		virtual Result CreateSwapChain(const SwapChainDesc& swapChainDesc, SwapChain*& swapChain) override;
		virtual void DestroySwapChain(SwapChain& swapChain) override;
		virtual Result GetDisplays(Display** displays, uint32_t& displayNum) override;
		virtual Result GetDisplaySize(Display& display, uint16_t& width, uint16_t& height) override;
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
		virtual Result AllocateMemory(uint32_t physicalDeviceMask, MemoryType memoryType, uint64_t size, Memory*& memory) override;
		virtual Result BindBufferMemory(const BufferMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override;
		virtual Result BindTextureMemory(const TextureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override;
		virtual void FreeMemory(Memory& memory);

		virtual FormatSupportBits GetFormatSupport(Format format) const;

		virtual uint32_t CalculateAllocationNumber(const ResourceGroupDesc& resourceGroupDesc) const override;
		virtual Result AllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations) override;

		//================================================================================================================
		// DeviceBase
		//================================================================================================================
		virtual void Destroy() override;

		///// Inherited via Device
		virtual Result CreateRayTracingPipeline(const RayTracingPipelineDesc& rayTracingPipelineDesc, Pipeline*& pipeline) override { return Result::UNSUPPORTED; }
		virtual Result CreateAccelerationStructure(const AccelerationStructureDesc& accelerationStructureDesc, AccelerationStructure*& accelerationStructure) override { return Result::UNSUPPORTED; }
		virtual void DestroyAccelerationStructure(AccelerationStructure& accelerationStructure) override {}
		virtual Result BindAccelerationStructureMemory(const AccelerationStructureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) override { return Result::UNSUPPORTED; }

	private:
		void InitVersionedDevice(ID3D11Device* device, bool isDeferredContextsEmulationRequested);
		void InitVersionedContext();

		template<typename Implementation, typename Interface, typename ConstructorArg, typename ... Args>
		nri::Result CreateImplementationWithNonEmptyConstructor(Interface*& entity, ConstructorArg&& constructorArg, const Args&... args);

	private:
		// don't sort - ~D3D11Extensions must be called last!
		D3D11Extensions m_Ext = {};
		VersionedDevice m_Device = {};
		VersionedContext m_ImmediateContext = {};
		Vector<CommandQueueD3D11> m_CommandQueues;
		DeviceDesc m_Desc = {};
		CRITICAL_SECTION m_CriticalSection = {};
		ComPtr<IDXGIAdapter> m_Adapter;

	private:
		void FillLimits(bool isValidationEnabled, Vendor vendor);
	};

	inline const VersionedContext& DeviceD3D11::GetImmediateContext() const
	{
		return m_ImmediateContext;
	}

	inline const VersionedDevice& DeviceD3D11::GetDevice() const
	{
		return m_Device;
	}
}

NRI_D3D11_API nri::Result CreateDeviceD3D11(const nri::DeviceCreationDesc& deviceCreationDesc, nri::Device*& device);
NRI_D3D11_API nri::Result CreateDeviceD3D11(const nri::DeviceCreationD3D11Desc& deviceDesc, nri::Device*& device);
