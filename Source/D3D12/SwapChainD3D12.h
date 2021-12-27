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

#include "SwapChain.h"

struct IDXGIFactory2;
struct ID3D12CommandQueue;
struct IDXGISwapChain4;

namespace nri
{
    struct NRI_D3D12_API SwapChainD3D12 : public SwapChain
    {
        SwapChainD3D12(DeviceD3D12& device);
        ~SwapChainD3D12();

        DeviceD3D12& GetDevice() const;

        Result Create(const SwapChainDesc& swapChainDesc);

        //================================================================================================================
        // NRI
        //================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual Texture* const* GetTextures(uint32_t& textureNum, Format& format) const override;
        virtual uint32_t AcquireNextTexture(QueueSemaphore& textureReadyForRender) override;
        virtual Result Present(QueueSemaphore& textureReadyForPresent) override;
        virtual Result SetHdrMetadata(const HdrMetadata& hdrMetadata) override;

    private:
        DeviceD3D12& m_Device;
        ComPtr<IDXGISwapChain4> m_SwapChain;
        ComPtr<ID3D12CommandQueue> m_CommandQueue;
        Vector<TextureD3D12> m_Textures;
        Vector<Texture*> m_TexturePointer;
        Format m_Format = Format::UNKNOWN;
        bool m_IsTearingAllowed = false;
        bool m_IsFullscreenEnabled = false;

        SwapChainDesc m_SwapChainDesc = {};
    };

    inline DeviceD3D12& SwapChainD3D12::GetDevice() const
    {
        return m_Device;
    }
}
