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
#include "CommandAllocator.h"

struct VersionedDevice;

namespace nri
{
    struct  NRI_D3D11_API CommandAllocatorD3D11 : public CommandAllocator
    {
        CommandAllocatorD3D11(DeviceD3D11& device, const VersionedDevice& versionedDevice);
        ~CommandAllocatorD3D11();

        inline DeviceD3D11& GetDevice() const
        { return m_Device; }

        //======================================================================================================================
        // NRI
        //======================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual Result CreateCommandBuffer(CommandBuffer*& commandBuffer) override;
        virtual void Reset() override;

    private:
        DeviceD3D11& m_Device;
    };
}

NRI_D3D11_API nri::Result CreateCommandBuffer(nri::DeviceD3D11& deviceImpl, ID3D11DeviceContext* precreatedContext, nri::CommandBuffer*& commandBuffer);
