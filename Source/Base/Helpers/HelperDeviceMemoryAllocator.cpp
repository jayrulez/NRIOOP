#include "BaseDLL.h"
#include "Helpers/HelperDeviceMemoryAllocator.h"
#include "Device.h"
#include "Buffer.h"
#include "Texture.h"

using namespace nri;

HelperDeviceMemoryAllocator::MemoryTypeGroup::MemoryTypeGroup(const StdAllocator<uint8_t>& stdAllocator) :
    buffers(stdAllocator),
    bufferOffsets(stdAllocator),
    textures(stdAllocator),
    textureOffsets(stdAllocator),
    memoryOffset(0)
{
}

HelperDeviceMemoryAllocator::HelperDeviceMemoryAllocator(Device& device, const StdAllocator<uint8_t>& stdAllocator) :
    m_Device(device),
    m_StdAllocator(stdAllocator),
    m_Map(stdAllocator),
    m_DedicatedBuffers(stdAllocator),
    m_DedicatedTextures(stdAllocator),
    m_BufferBindingDescs(stdAllocator),
    m_TextureBindingDescs(stdAllocator)
{
}

uint32_t HelperDeviceMemoryAllocator::CalculateAllocationNumber(const ResourceGroupDesc& resourceGroupDesc)
{
    m_Map.clear();
    m_DedicatedBuffers.clear();
    m_DedicatedTextures.clear();

    GroupByMemoryType(resourceGroupDesc.memoryLocation, resourceGroupDesc.buffers, resourceGroupDesc.bufferNum);
    GroupByMemoryType(resourceGroupDesc.memoryLocation, resourceGroupDesc.textures, resourceGroupDesc.textureNum);

    return uint32_t(m_Map.size()) + uint32_t(m_DedicatedBuffers.size()) + uint32_t(m_DedicatedTextures.size());
}

Result HelperDeviceMemoryAllocator::AllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations)
{
    m_Map.clear();
    m_DedicatedBuffers.clear();
    m_DedicatedTextures.clear();
    m_BufferBindingDescs.clear();
    m_TextureBindingDescs.clear();

    size_t allocationNum = 0;

    const Result result = TryToAllocateAndBindMemory(resourceGroupDesc, allocations, allocationNum);

    if (result != Result::SUCCESS)
    {
        for (size_t i = 0; i < allocationNum; i++)
        {
            m_Device.FreeMemory(*allocations[i]);
            allocations[i] = nullptr;
        }
    }

    return result;
}

Result HelperDeviceMemoryAllocator::TryToAllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations, size_t& allocationNum)
{
    GroupByMemoryType(resourceGroupDesc.memoryLocation, resourceGroupDesc.buffers, resourceGroupDesc.bufferNum);
    GroupByMemoryType(resourceGroupDesc.memoryLocation, resourceGroupDesc.textures, resourceGroupDesc.textureNum);

    Result result = Result::SUCCESS;

    for (auto it = m_Map.begin(); it != m_Map.end() && result == Result::SUCCESS; ++it)
        result = ProcessMemoryTypeGroup(it->first, it->second, allocations, allocationNum);

    if (result != Result::SUCCESS)
        return result;

    result = ProcessDedicatedResources(resourceGroupDesc.memoryLocation, allocations, allocationNum);

    if (result != Result::SUCCESS)
        return result;

    result = m_Device.BindBufferMemory(m_BufferBindingDescs.data(), (uint32_t)m_BufferBindingDescs.size());

    if (result != Result::SUCCESS)
        return result;

    result = m_Device.BindTextureMemory(m_TextureBindingDescs.data(), (uint32_t)m_TextureBindingDescs.size());

    return result;
}

Result HelperDeviceMemoryAllocator::ProcessMemoryTypeGroup(MemoryType memoryType, MemoryTypeGroup& group, Memory** allocations, size_t& allocationNum)
{
    Memory*& memory = allocations[allocationNum];

    const uint64_t allocationSize = group.memoryOffset;

    const Result result = m_Device.AllocateMemory(WHOLE_DEVICE_GROUP, memoryType, allocationSize, memory);
    if (result != Result::SUCCESS)
        return result;

    FillMemoryBindingDescs(group.buffers.data(), group.bufferOffsets.data(), (uint32_t)group.buffers.size(), *memory);
    FillMemoryBindingDescs(group.textures.data(), group.textureOffsets.data(), (uint32_t)group.textures.size(), *memory);
    allocationNum++;

    return Result::SUCCESS;
}

Result HelperDeviceMemoryAllocator::ProcessDedicatedResources(MemoryLocation memoryLocation, Memory** allocations, size_t& allocationNum)
{
    constexpr uint64_t zeroOffset = 0;
    MemoryDesc memoryDesc = {};

    for (size_t i = 0; i < m_DedicatedBuffers.size(); i++)
    {
        (*m_DedicatedBuffers[i]).GetMemoryInfo(memoryLocation, memoryDesc);

        Memory*& memory = allocations[allocationNum];

        const Result result = m_Device.AllocateMemory(WHOLE_DEVICE_GROUP, memoryDesc.type, memoryDesc.size, memory);
        if (result != Result::SUCCESS)
            return result;

        FillMemoryBindingDescs(m_DedicatedBuffers.data() + i, &zeroOffset, 1, *memory);
        allocationNum++;
    }

    for (size_t i = 0; i < m_DedicatedTextures.size(); i++)
    {
        (*m_DedicatedTextures[i]).GetMemoryInfo(memoryLocation, memoryDesc);

        Memory*& memory = allocations[allocationNum];

        const Result result = m_Device.AllocateMemory(WHOLE_DEVICE_GROUP, memoryDesc.type, memoryDesc.size, memory);
        if (result != Result::SUCCESS)
            return result;

        FillMemoryBindingDescs(m_DedicatedTextures.data() + i, &zeroOffset, 1, *memory);
        allocationNum++;
    }

    return Result::SUCCESS;
}

void HelperDeviceMemoryAllocator::GroupByMemoryType(MemoryLocation memoryLocation, Buffer* const* buffers, uint32_t bufferNum)
{
    MemoryDesc memoryDesc = {};

    for (uint32_t i = 0; i < bufferNum; i++)
    {
        Buffer* buffer = buffers[i];
        (*buffer).GetMemoryInfo(memoryLocation, memoryDesc);

        if (memoryDesc.mustBeDedicated)
            m_DedicatedBuffers.push_back(buffer);
        else
        {
            MemoryTypeGroup& group = m_Map.try_emplace(memoryDesc.type, m_StdAllocator).first->second;

            const uint64_t offset = GetAlignedSize(group.memoryOffset, memoryDesc.alignment);

            group.buffers.push_back(buffer);
            group.bufferOffsets.push_back(offset);
            group.memoryOffset = offset + memoryDesc.size;
        }
    }
}

void HelperDeviceMemoryAllocator::GroupByMemoryType(MemoryLocation memoryLocation, Texture* const* textures, uint32_t textureNum)
{
    const DeviceDesc& deviceDesc = m_Device.GetDesc();

    MemoryDesc memoryDesc = {};

    for (uint32_t i = 0; i < textureNum; i++)
    {
        Texture* texture = textures[i];
        (*texture).GetMemoryInfo(memoryLocation, memoryDesc);

        if (memoryDesc.mustBeDedicated)
            m_DedicatedTextures.push_back(texture);
        else
        {
            MemoryTypeGroup& group = m_Map.try_emplace(memoryDesc.type, m_StdAllocator).first->second;

            if (group.textures.empty() && group.memoryOffset > 0)
                group.memoryOffset = GetAlignedSize(group.memoryOffset, deviceDesc.bufferTextureGranularity);

            const uint64_t offset = GetAlignedSize(group.memoryOffset, memoryDesc.alignment);

            group.textures.push_back(texture);
            group.textureOffsets.push_back(offset);
            group.memoryOffset = offset + memoryDesc.size;
        }
    }
}

void HelperDeviceMemoryAllocator::FillMemoryBindingDescs(Buffer* const* buffers, const uint64_t* bufferOffsets, uint32_t bufferNum, Memory& memory)
{
    for (uint32_t i = 0; i < bufferNum; i++)
    {
        BufferMemoryBindingDesc desc = {};
        desc.memory = &memory;
        desc.buffer = buffers[i];
        desc.offset = bufferOffsets[i];
        desc.physicalDeviceMask = WHOLE_DEVICE_GROUP;

        m_BufferBindingDescs.push_back(desc);
    }
}

void HelperDeviceMemoryAllocator::FillMemoryBindingDescs(Texture* const* textures, const uint64_t* textureOffsets, uint32_t textureNum, Memory& memory)
{
    for (uint32_t i = 0; i < textureNum; i++)
    {
        TextureMemoryBindingDesc desc = {};
        desc.memory = &memory;
        desc.texture = textures[i];
        desc.offset = textureOffsets[i];
        desc.physicalDeviceMask = WHOLE_DEVICE_GROUP;

        m_TextureBindingDescs.push_back(desc);
    }
}
