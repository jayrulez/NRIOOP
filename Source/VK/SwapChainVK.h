/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include "VKDLL.h"

#include "SwapChain.h"

namespace nri
{
    struct NRI_VK_API SwapChainVK : public SwapChain
    {
        SwapChainVK(DeviceVK& device);
        ~SwapChainVK();

        DeviceVK& GetDevice() const;
        Result Create(const SwapChainDesc& swapChainDesc);

        virtual void SetDebugName(const char* name) override;
        virtual Texture* const* GetTextures(uint32_t& textureNum, Format& format) const override;
        virtual uint32_t AcquireNextTexture(QueueSemaphore& textureReadyForRender) override;
        virtual Result Present(QueueSemaphore& textureReadyForPresent) override;
        virtual Result SetHdrMetadata(const HdrMetadata& hdrMetadata) override;

    private:
        Result CreateSurface(const SwapChainDesc& swapChainDesc);

        VkSwapchainKHR m_Handle = VK_NULL_HANDLE;
        const CommandQueueVK* m_CommandQueue = nullptr;
        uint32_t m_TextureIndex = std::numeric_limits<uint32_t>::max();
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
        Vector<TextureVK*> m_Textures;
        Format m_Format = Format::UNKNOWN;
        DeviceVK& m_Device;
    };

    inline DeviceVK& SwapChainVK::GetDevice() const
    {
        return m_Device;
    }
}