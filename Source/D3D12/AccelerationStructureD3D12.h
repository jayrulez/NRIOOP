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

#include "AccelerationStructure.h"

#ifdef __ID3D12GraphicsCommandList4_INTERFACE_DEFINED__
namespace nri
{
    struct NRI_D3D12_API AccelerationStructureD3D12 : public AccelerationStructure
    {
        AccelerationStructureD3D12(DeviceD3D12& device);
        ~AccelerationStructureD3D12();

        DeviceD3D12& GetDevice() const;

        Result Create(const AccelerationStructureDesc& accelerationStructureDesc);
        Result BindMemory(Memory* memory, uint64_t offset);

        //================================================================================================================
        // NRI
        //================================================================================================================
        virtual void SetDebugName(const char* name) override;

        virtual Result CreateDescriptor(uint32_t physicalDeviceMask, Descriptor*& descriptor) const override;
        virtual void GetMemoryInfo(MemoryDesc& memoryDesc) const override;
        virtual uint64_t GetUpdateScratchBufferSize() const  override;
        virtual uint64_t GetBuildScratchBufferSize() const  override;
        virtual uint64_t GetHandle(uint32_t physicalDeviceIndex) const  override;

    private:
        DeviceD3D12& m_Device;
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO m_PrebuildInfo = {};
        BufferD3D12* m_Buffer = nullptr;
    };

    inline AccelerationStructureD3D12::AccelerationStructureD3D12(DeviceD3D12& device)
        : m_Device(device)
    {}

    inline DeviceD3D12& AccelerationStructureD3D12::GetDevice() const
    {
        return m_Device;
    }
}
#endif
