/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#include "Base.h"
#include "Device.h"
#include "DeviceVal.h"
#include "SharedVal.h"
#include "MemoryVal.h"
#include "DescriptorVal.h"
#include "AccelerationStructureVal.h"

using namespace nri;

AccelerationStructureVal::AccelerationStructureVal(DeviceVal& device, AccelerationStructure& accelerationStructure) :
    DeviceObjectVal(device, accelerationStructure)
{
}

AccelerationStructureVal::~AccelerationStructureVal()
{
    if (m_Memory != nullptr)
        m_Memory->UnbindAccelerationStructure(*this);

    m_Device.DestroyAccelerationStructure(m_ImplObject);
}

void AccelerationStructureVal::GetMemoryInfo(MemoryDesc& memoryDesc) const
{
    m_ImplObject.GetMemoryInfo(memoryDesc);
    m_Device.RegisterMemoryType(memoryDesc.type, MemoryLocation::DEVICE);
}

uint64_t AccelerationStructureVal::GetUpdateScratchBufferSize() const
{
    return m_ImplObject.GetUpdateScratchBufferSize();
}

uint64_t AccelerationStructureVal::GetBuildScratchBufferSize() const
{
    return m_ImplObject.GetBuildScratchBufferSize();
}

uint64_t AccelerationStructureVal::GetHandle(uint32_t physicalDeviceIndex) const
{
    RETURN_ON_FAILURE(m_Device.GetLog(), m_Memory != nullptr, 0,
        "Can't get AccelerationStructure handle: AccelerationStructure is not bound to memory.");

    return m_ImplObject.GetHandle(physicalDeviceIndex);
}

Result AccelerationStructureVal::CreateDescriptor(uint32_t physicalDeviceIndex, Descriptor*& descriptor)
{
    RETURN_ON_FAILURE(m_Device.GetLog(), physicalDeviceIndex < m_Device.GetPhysicalDeviceNum(), Result::INVALID_ARGUMENT,
        "Can't create Descriptor: 'physicalDeviceIndex' is invalid.");

    Descriptor* descriptorImpl = nullptr;
    const Result result = m_ImplObject.CreateDescriptor(physicalDeviceIndex, descriptorImpl);

    if (result == Result::SUCCESS)
    {
        RETURN_ON_FAILURE(m_Device.GetLog(), descriptorImpl != nullptr, Result::FAILURE, "Unexpected error: 'descriptorImpl' is nullptr.");
        descriptor = (Descriptor*)Allocate<DescriptorVal>(m_Device.GetStdAllocator(), m_Device, *descriptorImpl, ResourceType::ACCELERATION_STRUCTURE);
    }

    return result;
}

void AccelerationStructureVal::SetDebugName(const char* name)
{
    m_Name = name;
    m_ImplObject.SetDebugName(name);
}
