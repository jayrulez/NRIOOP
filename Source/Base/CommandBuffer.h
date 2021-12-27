#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API CommandBuffer
	{
	public:
		virtual ~CommandBuffer(){}

		virtual  void SetDebugName(const char* name) = 0;
		virtual  Result Begin(const DescriptorPool* descriptorPool, uint32_t physicalDeviceIndex) = 0;
		virtual  Result End() = 0;

		virtual   void SetPipeline(const Pipeline& pipeline) = 0;
		virtual  void SetPipelineLayout(const PipelineLayout& pipelineLayout) = 0;
		virtual  void SetDescriptorSets(uint32_t baseSlot, uint32_t descriptorSetNum, const DescriptorSet* const* descriptorSets, const uint32_t* dynamicConstantBufferOffsets) = 0;
		virtual   void SetConstants(uint32_t pushConstantIndex, const void* data, uint32_t size) = 0;
		virtual   void SetDescriptorPool(const DescriptorPool& descriptorPool) = 0;
		virtual   void PipelineBarrier(const TransitionBarrierDesc* transitionBarriers, const AliasingBarrierDesc* aliasingBarriers, BarrierDependency dependency) = 0;

		virtual   void BeginRenderPass(const FrameBuffer& frameBuffer, RenderPassBeginFlag renderPassBeginFlag) = 0;
		virtual  void EndRenderPass() = 0;
		virtual  void SetViewports(const Viewport* viewports, uint32_t viewportNum) = 0;
		virtual   void SetScissors(const Rect* rects, uint32_t rectNum) = 0;
		virtual   void SetDepthBounds(float boundsMin, float boundsMax) = 0;
		virtual   void SetStencilReference(uint8_t reference) = 0;
		virtual   void SetSamplePositions(const SamplePosition* positions, uint32_t positionNum) = 0;
		virtual   void ClearAttachments(const ClearDesc* clearDescs, uint32_t clearDescNum, const Rect* rects, uint32_t rectNum) = 0;
		virtual   void SetIndexBuffer(const Buffer& buffer, uint64_t offset, IndexType indexType) = 0;
		virtual   void SetVertexBuffers(uint32_t baseSlot, uint32_t bufferNum, const Buffer* const* buffers, const uint64_t* offsets) = 0;

		virtual    void Draw(uint32_t vertexNum, uint32_t instanceNum, uint32_t baseVertex, uint32_t baseInstance) = 0;
		virtual    void DrawIndexed(uint32_t indexNum, uint32_t instanceNum, uint32_t baseIndex, uint32_t baseVertex, uint32_t baseInstance) = 0;
		virtual    void DrawIndirect(const Buffer& buffer, uint64_t offset, uint32_t drawNum, uint32_t stride) = 0;
		virtual   void DrawIndexedIndirect(const Buffer& buffer, uint64_t offset, uint32_t drawNum, uint32_t stride) = 0;
		virtual    void Dispatch(uint32_t x, uint32_t y, uint32_t z) = 0;
		virtual   void DispatchIndirect(const Buffer& buffer, uint64_t offset) = 0;
		virtual   void BeginQuery(const QueryPool& queryPool, uint32_t offset) = 0;
		virtual   void EndQuery(const QueryPool& queryPool, uint32_t offset) = 0;
		virtual   void BeginAnnotation(const char* name) = 0;
		virtual   void EndAnnotation() = 0;

		virtual   void ClearStorageBuffer(const ClearStorageBufferDesc& clearDesc) = 0;

		virtual   void ClearStorageTexture(const ClearStorageTextureDesc& clearDesc) = 0;

		virtual   void CopyBuffer(Buffer& dstBuffer, uint32_t dstPhysicalDeviceIndex, uint64_t dstOffset, const Buffer& srcBuffer,
			uint32_t srcPhysicalDeviceIndex, uint64_t srcOffset, uint64_t size) = 0;

		virtual   void CopyTexture(Texture& dstTexture, uint32_t dstPhysicalDeviceIndex, const TextureRegionDesc* dstRegionDesc,
			const Texture& srcTexture, uint32_t srcPhysicalDeviceIndex, const TextureRegionDesc* srcRegionDesc) = 0;

		virtual   void UploadBufferToTexture(Texture& dstTexture, const TextureRegionDesc& dstRegionDesc, const Buffer& srcBuffer,
			const TextureDataLayoutDesc& srcDataLayoutDesc) = 0;

		virtual   void ReadbackTextureToBuffer(Buffer& dstBuffer, TextureDataLayoutDesc& dstDataLayoutDesc, const Texture& srcTexture,
			const TextureRegionDesc& srcRegionDesc) = 0;

		virtual   void CopyQueries(const QueryPool& queryPool, uint32_t offset, uint32_t num, Buffer& dstBuffer, uint64_t dstOffset) = 0;
		virtual   void ResetQueries(const QueryPool& queryPool, uint32_t offset, uint32_t num) = 0;

		virtual   void BuildTopLevelAccelerationStructure(uint32_t instanceNum, const Buffer& buffer, uint64_t bufferOffset,
			AccelerationStructureBuildBits flags, AccelerationStructure& dst, Buffer& scratch, uint64_t scratchOffset) = 0;

		virtual  void BuildBottomLevelAccelerationStructure(uint32_t geometryObjectNum, const GeometryObject* geometryObjects,
			AccelerationStructureBuildBits flags, AccelerationStructure& dst, Buffer& scratch, uint64_t scratchOffset) = 0;

		virtual   void UpdateTopLevelAccelerationStructure(uint32_t instanceNum, const Buffer& buffer, uint64_t bufferOffset,
			AccelerationStructureBuildBits flags, AccelerationStructure& dst, AccelerationStructure& src, Buffer& scratch,
			uint64_t scratchOffset) = 0;

		virtual   void UpdateBottomLevelAccelerationStructure(uint32_t geometryObjectNum, const GeometryObject* geometryObjects,
			AccelerationStructureBuildBits flags, AccelerationStructure& dst, AccelerationStructure& src, Buffer& scratch,
			uint64_t scratchOffset) = 0;

		virtual   void CopyAccelerationStructure(AccelerationStructure& dst, AccelerationStructure& src, CopyMode copyMode) = 0;

		virtual  void WriteAccelerationStructureSize(const AccelerationStructure* const* accelerationStructures,
			uint32_t accelerationStructureNum, QueryPool& queryPool, uint32_t queryPoolOffset) = 0;

		virtual  void DispatchRays(const DispatchRaysDesc& dispatchRaysDesc) = 0;
		virtual  void DispatchMeshTasks(uint32_t taskNum) = 0;
	};
}