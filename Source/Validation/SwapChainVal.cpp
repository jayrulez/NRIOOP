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
#include "SwapChainVal.h"

#include "QueueSemaphoreVal.h"
#include "TextureVal.h"

using namespace nri;

SwapChainVal::SwapChainVal(DeviceVal& device, SwapChain& swapChain, const SwapChainDesc& swapChainDesc) :
    DeviceObjectVal(device, swapChain),
    m_Textures(device.GetStdAllocator()),
    m_SwapChainDesc(swapChainDesc)
{
}

SwapChainVal::~SwapChainVal()
{
    for (size_t i = 0; i < m_Textures.size(); i++)
        Deallocate(m_Device.GetStdAllocator(), m_Textures[i]);
}

inline void SwapChainVal::SetDebugName(const char* name)
{
    m_Name = name;
    m_ImplObject.SetDebugName(name);
}

Texture* const* SwapChainVal::GetTextures(uint32_t& textureNum, Format& format) const
{
    Texture* const* textures = m_ImplObject.GetTextures(textureNum, format);

    TextureDesc textureDesc = {};
    textureDesc.type = TextureType::TEXTURE_2D;
    textureDesc.usageMask = TextureUsageBits::SHADER_RESOURCE | TextureUsageBits::COLOR_ATTACHMENT;
    textureDesc.format = format;
    textureDesc.size[0] = m_SwapChainDesc.width;
    textureDesc.size[1] = m_SwapChainDesc.height;
    textureDesc.size[2] = 1;
    textureDesc.mipNum = 1;
    textureDesc.arraySize = 1;
    textureDesc.sampleNum = 1;
    textureDesc.physicalDeviceMask = 0;

    m_Textures.resize(textureNum);
    for (uint32_t i = 0; i < textureNum; i++)
        m_Textures[i] = Allocate<TextureVal>(m_Device.GetStdAllocator(), m_Device, *textures[i], textureDesc);

    return (Texture* const*)m_Textures.data();
}

inline uint32_t SwapChainVal::AcquireNextTexture(QueueSemaphore& textureReadyForRender)
{
    ((QueueSemaphoreVal*)&textureReadyForRender)->Signal();
    QueueSemaphore* queueSemaphoreImpl = NRI_GET_IMPL(QueueSemaphore, &textureReadyForRender);

    return m_ImplObject.AcquireNextTexture(*queueSemaphoreImpl);
}

inline Result SwapChainVal::Present(QueueSemaphore& textureReadyForPresent)
{
    ((QueueSemaphoreVal*)&textureReadyForPresent)->Wait();
    QueueSemaphore* queueSemaphoreImpl = NRI_GET_IMPL(QueueSemaphore, &textureReadyForPresent);

    return m_ImplObject.Present(*queueSemaphoreImpl);
}

inline Result SwapChainVal::SetHdrMetadata(const HdrMetadata& hdrMetadata)
{
    return m_ImplObject.SetHdrMetadata(hdrMetadata);
}
