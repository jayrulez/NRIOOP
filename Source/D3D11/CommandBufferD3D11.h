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

namespace nri
{
    struct NRI_D3D11_API CommandBufferD3D11 final : public CommandBufferHelper
    {
        CommandBufferD3D11(DeviceD3D11& deviceImpl);
        CommandBufferD3D11(DeviceD3D11& deviceImpl, const VersionedContext& immediateContext);
        ~CommandBufferD3D11();

        inline const VersionedContext& GetContext() const
        {
            return m_Context;
        }

        //======================================================================================================================
        // nri::CommandBufferHelper
        //======================================================================================================================
        virtual Result Create(ID3D11DeviceContext* precreatedContext) override;
        virtual void Submit(const VersionedContext& context) override;
        virtual StdAllocator<uint8_t>& GetStdAllocator() const override;

        //======================================================================================================================
        // NRI
        //======================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual Result Begin(const DescriptorPool* descriptorPool, uint32_t physicalDeviceIndex) override;
        virtual Result End() override;
        virtual void SetViewports(const Viewport* viewports, uint32_t viewportNum) override;
        virtual void SetScissors(const Rect* rects, uint32_t rectNum) override;
        virtual void SetDepthBounds(float boundsMin, float boundsMax) override;
        virtual void SetStencilReference(uint8_t reference) override;
        virtual void SetSamplePositions(const SamplePosition* positions, uint32_t positionNum) override;
        virtual void ClearAttachments(const ClearDesc* clearDescs, uint32_t clearDescNum, const Rect* rects, uint32_t rectNum) override;
        virtual void ClearStorageBuffer(const ClearStorageBufferDesc& clearDesc) override;
        virtual void ClearStorageTexture(const ClearStorageTextureDesc& clearDesc) override;
        virtual void BeginRenderPass(const FrameBuffer& frameBuffer, RenderPassBeginFlag renderPassBeginFlag) override;
        virtual  void EndRenderPass() override;
        virtual void SetVertexBuffers(uint32_t baseSlot, uint32_t bufferNum, const Buffer* const* buffers, const uint64_t* offsets) override;
        virtual void SetIndexBuffer(const Buffer& buffer, uint64_t offset, IndexType indexType) override;
        virtual void SetPipelineLayout(const PipelineLayout& pipelineLayout) override;
        virtual void SetPipeline(const Pipeline& pipeline) override;
        virtual void SetDescriptorPool(const DescriptorPool& descriptorPool) override;
        virtual void SetDescriptorSets(uint32_t baseIndex, uint32_t descriptorSetNum, const DescriptorSet* const* descriptorSets, const uint32_t* dynamicConstantBufferOffsets) override;
        virtual void SetConstants(uint32_t pushConstantIndex, const void* data, uint32_t size) override;
        virtual  void Draw(uint32_t vertexNum, uint32_t instanceNum, uint32_t baseVertex, uint32_t baseInstance) override;
        virtual void DrawIndexed(uint32_t indexNum, uint32_t instanceNum, uint32_t baseIndex, uint32_t baseVertex, uint32_t baseInstance) override;
        virtual void DrawIndirect(const Buffer& buffer, uint64_t offset, uint32_t drawNum, uint32_t stride) override;
        virtual void DrawIndexedIndirect(const Buffer& buffer, uint64_t offset, uint32_t drawNum, uint32_t stride) override;
        virtual void CopyBuffer(Buffer& dstBuffer, uint32_t dstPhysicalDeviceIndex, uint64_t dstOffset, const Buffer& srcBuffer, uint32_t srcPhysicalDeviceIndex, uint64_t srcOffset, uint64_t size) override;
        virtual  void CopyTexture(Texture& dstTexture, uint32_t dstPhysicalDeviceIndex, const TextureRegionDesc* dstRegionDesc, const Texture& srcTexture, uint32_t srcPhysicalDeviceIndex, const TextureRegionDesc* srcRegionDesc) override;
        virtual  void UploadBufferToTexture(Texture& dstTexture, const TextureRegionDesc& dstRegionDesc, const Buffer& srcBuffer, const TextureDataLayoutDesc& srcDataLayoutDesc) override;
        virtual  void ReadbackTextureToBuffer(Buffer& dstBuffer, TextureDataLayoutDesc& dstDataLayoutDesc, const Texture& srcTexture, const TextureRegionDesc& srcRegionDesc) override;
        virtual  void Dispatch(uint32_t x, uint32_t y, uint32_t z) override;
        virtual  void DispatchIndirect(const Buffer& buffer, uint64_t offset) override;
        virtual  void PipelineBarrier(const TransitionBarrierDesc* transitionBarriers, const AliasingBarrierDesc* aliasingBarriers, BarrierDependency dependency) override;
        virtual void BeginQuery(const QueryPool& queryPool, uint32_t offset) override;
        virtual void EndQuery(const QueryPool& queryPool, uint32_t offset) override;
        virtual void CopyQueries(const QueryPool& queryPool, uint32_t offset, uint32_t num, Buffer& dstBuffer, uint64_t dstOffset) override;
        virtual void BeginAnnotation(const char* name) override;
        virtual void EndAnnotation() override;


        virtual   void ResetQueries(const QueryPool& queryPool, uint32_t offset, uint32_t num) override{}
        virtual   void BuildTopLevelAccelerationStructure(uint32_t instanceNum, const Buffer& buffer, uint64_t bufferOffset,
            AccelerationStructureBuildBits flags, AccelerationStructure& dst, Buffer& scratch, uint64_t scratchOffset) override {}

        virtual  void BuildBottomLevelAccelerationStructure(uint32_t geometryObjectNum, const GeometryObject* geometryObjects,
            AccelerationStructureBuildBits flags, AccelerationStructure& dst, Buffer& scratch, uint64_t scratchOffset) override {}

        virtual   void UpdateTopLevelAccelerationStructure(uint32_t instanceNum, const Buffer& buffer, uint64_t bufferOffset,
            AccelerationStructureBuildBits flags, AccelerationStructure& dst, AccelerationStructure& src, Buffer& scratch,
            uint64_t scratchOffset) override {}

        virtual   void UpdateBottomLevelAccelerationStructure(uint32_t geometryObjectNum, const GeometryObject* geometryObjects,
            AccelerationStructureBuildBits flags, AccelerationStructure& dst, AccelerationStructure& src, Buffer& scratch,
            uint64_t scratchOffset) override {}

        virtual   void CopyAccelerationStructure(AccelerationStructure& dst, AccelerationStructure& src, CopyMode copyMode) override {}

        virtual  void WriteAccelerationStructureSize(const AccelerationStructure* const* accelerationStructures,
            uint32_t accelerationStructureNum, QueryPool& queryPool, uint32_t queryPoolOffset)override {}

        virtual  void DispatchRays(const DispatchRaysDesc& dispatchRaysDesc) override {}
        virtual  void DispatchMeshTasks(uint32_t taskNum) override {}
    private:
        const VersionedDevice& m_Device;
        VersionedContext m_Context = {};
        BindingState m_BindingState = {};
        SamplePositionsState m_SamplePositionsState = {};
        ComPtr<ID3D11CommandList> m_CommandList;
        ComPtr<ID3DUserDefinedAnnotation> m_Annotation;
        const FrameBufferD3D11* m_CurrentFrameBuffer = nullptr;
        PipelineLayoutD3D11* m_CurrentPipelineLayout = nullptr;
        PipelineD3D11* m_CurrentPipeline = nullptr;
        const Buffer* m_CurrentIndexBuffer = nullptr;
        const Buffer* m_CurrentVertexBuffer = nullptr;
        uint64_t m_CurrentIndexBufferOffset = 0;
        uint64_t m_CurrentVertexBufferOffset = 0;
        uint32_t m_CurrentVertexBufferBaseSlot = 0;
        IndexType m_CurrentIndexType = IndexType::UINT32;
        float m_DepthBounds[2] = {0.0f, 1.0f};
        uint8_t m_StencilRef = 0;
        DeviceD3D11& m_DeviceImpl;
    };
}
