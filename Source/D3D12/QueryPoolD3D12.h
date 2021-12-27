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

#include "QueryPool.h"

namespace nri
{
    struct  NRI_D3D12_API QueryPoolD3D12 : public QueryPool
    {
        QueryPoolD3D12(DeviceD3D12& device);
        ~QueryPoolD3D12();

        operator ID3D12QueryHeap*() const;

        DeviceD3D12& GetDevice() const;

        Result Create(const QueryPoolDesc& queryPoolDesc);
        D3D12_QUERY_TYPE GetType() const;
        ID3D12Resource* GetReadbackBuffer() const;

        //======================================================================================================================
        // NRI
        //======================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual uint32_t GetQuerySize() const override;

    private:
        Result CreateReadbackBuffer(const QueryPoolDesc& queryPoolDesc);

        DeviceD3D12& m_Device;
        D3D12_QUERY_TYPE m_QueryType = D3D12_QUERY_TYPE(-1);
        uint32_t m_QuerySize = 0;
        ComPtr<ID3D12QueryHeap> m_QueryHeap;
        ComPtr<ID3D12Resource> m_ReadbackBuffer;
    };

    inline QueryPoolD3D12::QueryPoolD3D12(DeviceD3D12& device)
        : m_Device(device)
    {}

    inline QueryPoolD3D12::~QueryPoolD3D12()
    {}

    inline QueryPoolD3D12::operator ID3D12QueryHeap*() const
    {
        return m_QueryHeap.GetInterface();
    }

    inline D3D12_QUERY_TYPE QueryPoolD3D12::GetType() const
    {
        return m_QueryType;
    }

    inline ID3D12Resource* QueryPoolD3D12::GetReadbackBuffer() const
    {
        return m_ReadbackBuffer.GetInterface();
    }

    inline DeviceD3D12& QueryPoolD3D12::GetDevice() const
    {
        return m_Device;
    }

    inline uint32_t QueryPoolD3D12::GetQuerySize() const
    {
        return m_QuerySize;
    }
}
