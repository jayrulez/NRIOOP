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

#include "QueueSemaphore.h"

struct ID3D12Device;
struct ID3D12CommandQueue;
struct ID3D12Fence;

namespace nri
{
    struct  NRI_D3D12_API QueueSemaphoreD3D12 : public QueueSemaphore
    {
        QueueSemaphoreD3D12(DeviceD3D12& device);
        ~QueueSemaphoreD3D12();

        Result Create();

        operator ID3D12Fence*() const;

        DeviceD3D12& GetDevice() const;

        void Signal(ID3D12CommandQueue* commandQueue);
        void Wait(ID3D12CommandQueue* commandQueue);

        //================================================================================================================
        // NRI
        //================================================================================================================
        virtual void SetDebugName(const char* name) override;

    private:
        DeviceD3D12& m_Device;
        ComPtr<ID3D12Fence> m_Fence;
        uint64_t m_SignalValue = 0;
    };

    inline QueueSemaphoreD3D12::QueueSemaphoreD3D12(DeviceD3D12& device)
        : m_Device(device)
    {}

    inline QueueSemaphoreD3D12::~QueueSemaphoreD3D12()
    {}

    inline QueueSemaphoreD3D12::operator ID3D12Fence*() const
    {
        return m_Fence.GetInterface();
    }

    inline DeviceD3D12& QueueSemaphoreD3D12::GetDevice() const
    {
        return m_Device;
    }
}
