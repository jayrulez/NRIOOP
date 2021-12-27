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

#include "CommandAllocator.h"

struct ID3D12CommandAllocator;
enum D3D12_COMMAND_LIST_TYPE;

namespace nri
{
    struct  NRI_D3D12_API CommandAllocatorD3D12 : public CommandAllocator
    {
        CommandAllocatorD3D12(DeviceD3D12& device);
        ~CommandAllocatorD3D12();

        operator ID3D12CommandAllocator*() const;

        DeviceD3D12& GetDevice() const;

        Result Create(const CommandQueue& commandQueue);

        //================================================================================================================
        // NRI
        //================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual Result CreateCommandBuffer(CommandBuffer*& commandBuffer) override;
        virtual void Reset() override;

    private:
        DeviceD3D12& m_Device;
        ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
        D3D12_COMMAND_LIST_TYPE m_CommandListType = D3D12_COMMAND_LIST_TYPE(-1);
    };

    inline CommandAllocatorD3D12::CommandAllocatorD3D12(DeviceD3D12& device)
        : m_Device(device)
    {}

    inline CommandAllocatorD3D12::~CommandAllocatorD3D12()
    {}

    inline CommandAllocatorD3D12::operator ID3D12CommandAllocator*() const
    {
        return m_CommandAllocator.GetInterface();
    }

    inline DeviceD3D12& CommandAllocatorD3D12::GetDevice() const
    {
        return m_Device;
    }
}
