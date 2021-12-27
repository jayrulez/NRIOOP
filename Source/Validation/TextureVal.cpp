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
#include "TextureVal.h"

using namespace nri;

TextureVal::TextureVal(DeviceVal& device, Texture& texture, const TextureDesc& textureDesc) :
    DeviceObjectVal(device, texture),
    m_TextureDesc(textureDesc)
{
}

TextureVal::~TextureVal()
{
    if (m_Memory != nullptr)
        m_Memory->UnbindTexture(*this);
}

void TextureVal::SetDebugName(const char* name)
{
    m_Name = name;
    m_ImplObject.SetDebugName(name);
}

void TextureVal::GetMemoryInfo(MemoryLocation memoryLocation, MemoryDesc& memoryDesc) const
{
    m_ImplObject.GetMemoryInfo(memoryLocation, memoryDesc);
    m_Device.RegisterMemoryType(memoryDesc.type, memoryLocation);
}
