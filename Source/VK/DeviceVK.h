/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#if _WIN32
#include <dxgi.h>
#endif

#include "VKDLL.h"

#include "Device.h"
#include "SharedVK.h"
#include "Lock.h"
#include "SharedLibrary.h"

namespace nri
{
	struct NRI_VK_API DeviceVK final : public Device
	{
		DeviceVK(const Logger& log, const StdAllocator<uint8_t>& stdAllocator);
		~DeviceVK();

		operator VkDevice() const;
		operator VkPhysicalDevice() const;
		operator VkInstance() const;

		const DispatchTable& GetDispatchTable() const;
		const VkAllocationCallbacks* GetAllocationCallbacks() const;
		const std::array<uint32_t, COMMAND_QUEUE_TYPE_NUM>& GetQueueFamilyIndices() const;
		const SPIRVBindingOffsets& GetSPIRVBindingOffsets() const;

		Result Create(const DeviceCreationVulkanDesc& deviceCreationVulkanDesc);
		Result Create(const DeviceCreationDesc& deviceCreationDesc);
		bool GetMemoryType(MemoryLocation memoryLocation, uint32_t memoryTypeMask, MemoryTypeInfo& memoryTypeInfo) const;
		bool GetMemoryType(uint32_t index, MemoryTypeInfo& memoryTypeInfo) const;

		uint32_t GetPhyiscalDeviceGroupSize() const;
		bool IsDescriptorIndexingExtSupported() const;
		bool IsConcurrentSharingModeEnabledForBuffers() const;
		bool IsConcurrentSharingModeEnabledForImages() const;
		const Vector<uint32_t>& GetConcurrentSharingModeQueueIndices() const;

		void SetDebugNameToTrivialObject(VkObjectType objectType, uint64_t handle, const char* name);
		void SetDebugNameToDeviceGroupObject(VkObjectType objectType, const uint64_t* handles, const char* name);

		virtual  void SetDebugName(const char* name) override;
		virtual  const DeviceDesc& GetDesc() const override;
		virtual   Result GetCommandQueue(CommandQueueType commandQueueType, CommandQueue*& commandQueue)override;

		virtual  Result CreateCommandAllocator(const CommandQueue& commandQueue, uint32_t physicalDeviceMask, CommandAllocator*& commandAllocator)override;
		virtual   Result CreateDescriptorPool(const DescriptorPoolDesc& descriptorPoolDesc, DescriptorPool*& descriptorPool)override;
		virtual   Result CreateBuffer(const BufferDesc& bufferDesc, Buffer*& buffer)override;
		virtual   Result CreateTexture(const TextureDesc& textureDesc, Texture*& texture)override;
		virtual  Result CreateBufferView(const BufferViewDesc& bufferViewDesc, Descriptor*& bufferView)override;
		virtual   Result CreateTexture1DView(const Texture1DViewDesc& textureViewDesc, Descriptor*& textureView)override;
		virtual   Result CreateTexture2DView(const Texture2DViewDesc& textureViewDesc, Descriptor*& textureView)override;
		virtual    Result CreateTexture3DView(const Texture3DViewDesc& textureViewDesc, Descriptor*& textureView)override;
		virtual  Result CreateSampler(const SamplerDesc& samplerDesc, Descriptor*& sampler)override;
		virtual  Result CreatePipelineLayout(const PipelineLayoutDesc& pipelineLayoutDesc, PipelineLayout*& pipelineLayout)override;
		virtual  Result CreateGraphicsPipeline(const GraphicsPipelineDesc& graphicsPipelineDesc, Pipeline*& pipeline)override;
		virtual  Result CreateComputePipeline(const ComputePipelineDesc& computePipelineDesc, Pipeline*& pipeline)override;
		virtual  Result CreateFrameBuffer(const FrameBufferDesc& frameBufferDesc, FrameBuffer*& frameBuffer)override;
		virtual   Result CreateQueryPool(const QueryPoolDesc& queryPoolDesc, QueryPool*& queryPool)override;
		virtual   Result CreateQueueSemaphore(QueueSemaphore*& queueSemaphore)override;
		virtual  Result CreateDeviceSemaphore(bool signaled, DeviceSemaphore*& deviceSemaphore)override;
		virtual  Result CreateSwapChain(const SwapChainDesc& swapChainDesc, SwapChain*& swapChain)override;
		virtual  Result CreateRayTracingPipeline(const RayTracingPipelineDesc& rayTracingPipelineDesc, Pipeline*& pipeline)override;
		virtual   Result CreateAccelerationStructure(const AccelerationStructureDesc& accelerationStructureDesc, AccelerationStructure*& accelerationStructure)override;

		Result CreateCommandQueue(const CommandQueueVulkanDesc& commandQueueDesc, CommandQueue*& commandQueue);
		Result CreateCommandAllocator(const CommandAllocatorVulkanDesc& commandAllocatorDesc, CommandAllocator*& commandAllocator);
		Result CreateCommandBuffer(const CommandBufferVulkanDesc& commandBufferDesc, CommandBuffer*& commandBuffer);
		Result CreateDescriptorPool(NRIVkDescriptorPool vkDescriptorPool, DescriptorPool*& descriptorPool);
		Result CreateBuffer(const BufferVulkanDesc& bufferDesc, Buffer*& buffer);
		Result CreateTexture(const TextureVulkanDesc& textureVulkanDesc, Texture*& texture);
		Result CreateMemory(const MemoryVulkanDesc& memoryVulkanDesc, Memory*& memory);
		Result CreateGraphicsPipeline(NRIVkPipeline vkPipeline, Pipeline*& pipeline);
		Result CreateComputePipeline(NRIVkPipeline vkPipeline, Pipeline*& pipeline);
		Result CreateQueryPool(const QueryPoolVulkanDesc& queryPoolVulkanDesc, QueryPool*& queryPool);
		Result CreateQueueSemaphore(NRIVkSemaphore vkSemaphore, QueueSemaphore*& queueSemaphore);
		virtual  Result CreateDeviceSemaphore(NRIVkFence vkFence, DeviceSemaphore*& deviceSemaphore);

		virtual   void DestroyCommandAllocator(CommandAllocator& commandAllocator)override;
		virtual   void DestroyCommandBuffer(CommandBuffer& commandBuffer)override;
		virtual   void DestroyDescriptorPool(DescriptorPool& descriptorPool)override;
		virtual  void DestroyBuffer(Buffer& buffer)override;
		virtual  void DestroyTexture(Texture& texture)override;
		virtual   void DestroyDescriptor(Descriptor& descriptor)override;
		virtual   void DestroyPipelineLayout(PipelineLayout& pipelineLayout)override;
		virtual  void DestroyPipeline(Pipeline& pipeline)override;
		virtual  void DestroyFrameBuffer(FrameBuffer& frameBuffer)override;
		virtual   void DestroyQueryPool(QueryPool& queryPool)override;
		virtual   void DestroyQueueSemaphore(QueueSemaphore& queueSemaphore)override;
		virtual  void DestroyDeviceSemaphore(DeviceSemaphore& deviceSemaphore)override;
		virtual  void DestroySwapChain(SwapChain& swapChain)override;
		virtual  void DestroyAccelerationStructure(AccelerationStructure& accelerationStructure)override;

		virtual   Result GetDisplays(Display** displays, uint32_t& displayNum)override;
		virtual   Result GetDisplaySize(Display& display, uint16_t& width, uint16_t& height)override;

		virtual   Result AllocateMemory(uint32_t physicalDeviceMask, MemoryType memoryType, uint64_t size, Memory*& memory)override;
		virtual   Result BindBufferMemory(const BufferMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum)override;
		virtual   Result BindTextureMemory(const TextureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum)override;
		virtual   Result BindAccelerationStructureMemory(const AccelerationStructureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum)override;
		virtual   void FreeMemory(Memory& memory)override;

		virtual  FormatSupportBits GetFormatSupport(Format format) const override;

		virtual  uint32_t CalculateAllocationNumber(const ResourceGroupDesc& resourceGroupDesc) const override;
		virtual Result AllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations)override;

		//================================================================================================================
		// DeviceBase
		//================================================================================================================
		virtual void Destroy() override;

	private:
		Result CreateInstance(const DeviceCreationDesc& deviceCreationDesc);
		Result FindPhysicalDeviceGroup(const PhysicalDeviceGroup* physicalDeviceGroup, bool enableMGPU);
		Result CreateLogicalDevice(const DeviceCreationDesc& deviceCreationDesc);
		void FillFamilyIndices(bool useEnabledFamilyIndices, const uint32_t* enabledFamilyIndices, uint32_t familyIndexNum);
		void SetDeviceLimits(bool enableValidation);
		void CreateCommandQueues();
		Result ResolvePreInstanceDispatchTable();
		Result ResolveInstanceDispatchTable();
		Result ResolveDispatchTable();
		void FilterInstanceLayers(Vector<const char*>& layers);
		bool FilterInstanceExtensions(Vector<const char*>& extensions);
		bool FilterDeviceExtensions(Vector<const char*>& extensions);
		void RetrieveRayTracingInfo();
		void RetrieveMeshShaderInfo();
		void ReportDeviceGroupInfo();
		void CheckSupportedDeviceExtensions(const Vector<const char*>& extensions);
		void FindDXGIAdapter();

		template< typename Implementation, typename Interface, typename ... Args >
		Result CreateImplementation(Interface*& entity, const Args&... args);

		VkDevice m_Device = VK_NULL_HANDLE;
		Vector<VkPhysicalDevice> m_PhysicalDevices;
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkPhysicalDeviceMemoryProperties m_MemoryProps = {};
		DispatchTable m_VK = {};
		DeviceDesc m_DeviceDesc = {};
		VkPhysicalDeviceRayTracingPipelinePropertiesKHR m_RayTracingDeviceProperties = {};
		std::array<uint32_t, COMMAND_QUEUE_TYPE_NUM> m_FamilyIndices = {};
		std::array<CommandQueueVK*, COMMAND_QUEUE_TYPE_NUM> m_Queues = {};
		Vector<uint32_t> m_PhysicalDeviceIndices;
		Vector<uint32_t> m_ConcurrentSharingModeQueueIndices;
		VkAllocationCallbacks* m_AllocationCallbackPtr = nullptr;
		VkAllocationCallbacks m_AllocationCallbacks = {};
		VkDebugUtilsMessengerEXT m_Messenger = VK_NULL_HANDLE;
		SPIRVBindingOffsets m_SPIRVBindingOffsets = {};
		Lock m_Lock;
		uint64_t m_LUID = 0;
		bool m_OwnsNativeObjects = false;
		bool m_IsRayTracingExtSupported = false;
		bool m_IsDescriptorIndexingExtSupported = false;
		bool m_IsSampleLocationExtSupported = false;
		bool m_IsMinMaxFilterExtSupported = false;
		bool m_IsConservativeRasterExtSupported = false;
		bool m_IsMeshShaderExtSupported = false;
		bool m_IsHDRExtSupported = false;
		bool m_IsDemoteToHelperInvocationSupported = false;
		bool m_IsSubsetAllocationSupported = false;
		bool m_IsConcurrentSharingModeEnabledForBuffers = true;
		bool m_IsConcurrentSharingModeEnabledForImages = true;
		Library* m_Loader = nullptr;
#if _WIN32
		ComPtr<IDXGIAdapter> m_Adapter;
#endif
	};

	inline DeviceVK::operator VkDevice() const
	{
		return m_Device;
	}

	inline DeviceVK::operator VkPhysicalDevice() const
	{
		return m_PhysicalDevices.front();
	}

	inline DeviceVK::operator VkInstance() const
	{
		return m_Instance;
	}

	inline const DispatchTable& DeviceVK::GetDispatchTable() const
	{
		return m_VK;
	}

	inline const VkAllocationCallbacks* DeviceVK::GetAllocationCallbacks() const
	{
		return m_AllocationCallbackPtr;
	}

	inline const std::array<uint32_t, COMMAND_QUEUE_TYPE_NUM>& DeviceVK::GetQueueFamilyIndices() const
	{
		return m_FamilyIndices;
	}

	inline const SPIRVBindingOffsets& DeviceVK::GetSPIRVBindingOffsets() const
	{
		return m_SPIRVBindingOffsets;
	}

	inline uint32_t DeviceVK::GetPhyiscalDeviceGroupSize() const
	{
		return m_DeviceDesc.phyiscalDeviceGroupSize;
	}

	inline bool DeviceVK::IsDescriptorIndexingExtSupported() const
	{
		return m_IsDescriptorIndexingExtSupported;
	}

	inline bool DeviceVK::IsConcurrentSharingModeEnabledForBuffers() const
	{
		return m_IsConcurrentSharingModeEnabledForBuffers;
	}

	inline bool DeviceVK::IsConcurrentSharingModeEnabledForImages() const
	{
		return m_IsConcurrentSharingModeEnabledForImages;
	}

	inline const Vector<uint32_t>& DeviceVK::GetConcurrentSharingModeQueueIndices() const
	{
		return m_ConcurrentSharingModeQueueIndices;
	}
}

NRI_VK_API nri::Result CreateDeviceVK(const nri::DeviceCreationDesc& deviceCreationDesc, nri::Device*& device);
NRI_VK_API nri::Result CreateDeviceVK(const nri::DeviceCreationVulkanDesc& deviceDesc, nri::Device*& device);

/*
	TODO:
		- Cubemaps and 3D textures
			Some flags are missing in VkImageCreateInfo
*/