/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include "ValidationDLL.h"
#include "SharedVal.h"

#include "Texture.h"

namespace nri
{
    struct MemoryVal;

    struct NRI_VALIDATION_API TextureVal : public DeviceObjectVal<Texture>
    {
        TextureVal(DeviceVal& device, Texture& texture, const TextureDesc& textureDesc);
        ~TextureVal();

        void SetBoundToMemory();
        void SetBoundToMemory(MemoryVal& memory);
        bool IsBoundToMemory() const;

        const TextureDesc& GetDesc() const;

        void SetDebugName(const char* name);
        void GetMemoryInfo(MemoryLocation memoryLocation, MemoryDesc& memoryDesc) const;

    private:
        MemoryVal* m_Memory = nullptr;
        bool m_IsBoundToMemory = false;
        TextureDesc m_TextureDesc = {};
    };

    inline void TextureVal::SetBoundToMemory()
    {
        m_IsBoundToMemory = true;
    }

    inline void TextureVal::SetBoundToMemory(MemoryVal& memory)
    {
        m_Memory = &memory;
        m_IsBoundToMemory = true;
    }

    inline bool TextureVal::IsBoundToMemory() const
    {
        return m_IsBoundToMemory;
    }

    inline const TextureDesc& TextureVal::GetDesc() const
    {
        return m_TextureDesc;
    }
}
