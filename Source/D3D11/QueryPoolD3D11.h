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

#include "QueryPool.h"

namespace nri
{
    struct NRI_D3D11_API QueryPoolD3D11 : public QueryPool
    {
        QueryPoolD3D11(DeviceD3D11& device);
        ~QueryPoolD3D11();

        inline DeviceD3D11& GetDevice() const
        { return m_Device; }

        Result Create(const VersionedDevice& device, const QueryPoolDesc& queryPoolDesc);
        void BeginQuery(const VersionedContext& context, uint32_t offset);
        void EndQuery(const VersionedContext& context, uint32_t offset);
        void GetData(uint8_t* dstMemory, const VersionedContext& immediateContext, uint32_t offset, uint32_t num) const;

        //======================================================================================================================
        // NRI
        //======================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual uint32_t GetQuerySize() const override;

    private:
        Vector<ComPtr<ID3D11Query>> m_Pool;
        DeviceD3D11& m_Device;
        QueryType m_Type = QueryType::MAX_NUM;
    };
}
