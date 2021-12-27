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
#include "BufferVal.h"

using namespace nri;

BufferVal::BufferVal(DeviceVal& device, Buffer& buffer, const BufferDesc& bufferDesc) :
    DeviceObjectVal(device, buffer),
    m_BufferDesc(bufferDesc)
{
}

BufferVal::~BufferVal()
{
    if (m_Memory != nullptr)
        m_Memory->UnbindBuffer(*this);
}

void BufferVal::SetDebugName(const char* name)
{
    m_Name = name;
    m_ImplObject.SetDebugName(name);
}

void BufferVal::GetMemoryInfo(MemoryLocation memoryLocation, MemoryDesc& memoryDesc) const
{
    m_ImplObject.GetMemoryInfo(memoryLocation, memoryDesc);
    m_Device.RegisterMemoryType(memoryDesc.type, memoryLocation);
}

void* BufferVal::Map(uint64_t offset, uint64_t size)
{
    RETURN_ON_FAILURE(m_Device.GetLog(), m_IsBoundToMemory, nullptr,
        "Can't map Buffer: Buffer is not bound to memory.");

    RETURN_ON_FAILURE(m_Device.GetLog(), !m_IsMapped, nullptr,
        "Can't map Buffer: the buffer is already mapped.");

    m_IsMapped = true;

    return m_ImplObject.Map(offset, size);
}

void BufferVal::Unmap()
{
    RETURN_ON_FAILURE(m_Device.GetLog(), m_IsMapped, ReturnVoid(),
        "Can't unmap Buffer: the buffer is not mapped.");

    m_IsMapped = false;

    m_ImplObject.Unmap();
}
