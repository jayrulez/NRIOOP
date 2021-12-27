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

#include "Buffer.h"


struct ID3D11Buffer;

namespace nri
{
    struct QueryRange
    {
        const QueryPoolD3D11* pool;
        uint32_t offset;
        uint32_t num;
        uint64_t bufferOffset;
    };

    struct  NRI_D3D11_API BufferD3D11 : public Buffer
    {
        BufferD3D11(DeviceD3D11& device, const VersionedContext& immediateContext, const BufferDesc& bufferDesc);
        BufferD3D11(DeviceD3D11& device, const VersionedContext& immediateContext);
        ~BufferD3D11();

        inline operator ID3D11Buffer*() const
        { return m_Buffer; }

        inline const BufferDesc& GetDesc() const
        { return m_Desc; }

        inline DeviceD3D11& GetDevice() const
        { return m_Device; }

        inline void AssignQueryPoolRange(const QueryPoolD3D11* queryPool, uint32_t offset, uint32_t num, uint64_t bufferOffset)
        {
            m_QueryRange.pool = queryPool;
            m_QueryRange.offset = offset;
            m_QueryRange.num = num;
            m_QueryRange.bufferOffset = bufferOffset;
        }

        Result Create(const VersionedDevice& device, const MemoryD3D11& memory);
        Result Create(const BufferD3D11Desc& bufferDesc);
        void* Map(MapType mapType, uint64_t offset);
        void FinalizeQueries();
        void FinalizeReadback();
        TextureD3D11& RecreateReadbackTexture(const VersionedDevice& device, const TextureD3D11& srcTexture, const TextureRegionDesc& srcRegionDesc, const TextureDataLayoutDesc& readbackDataLayoutDesc);

        //======================================================================================================================
        // NRI
        //======================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual void GetMemoryInfo(MemoryLocation memoryLocation, MemoryDesc& memoryDesc) const override;
        virtual void* Map(uint64_t offset, uint64_t size) override;
        virtual void Unmap() override;

    private:
        const VersionedContext& m_ImmediateContext;
        ComPtr<ID3D11Buffer> m_Buffer;
        TextureD3D11* m_ReadbackTexture = nullptr;
        BufferDesc m_Desc = {};
        BufferType m_Type = BufferType::DEVICE;
        QueryRange m_QueryRange = {};
        bool m_IsReadbackDataChanged = false;
        TextureDataLayoutDesc m_ReadbackDataLayoutDesc = {};
        DeviceD3D11& m_Device;
    };
}
