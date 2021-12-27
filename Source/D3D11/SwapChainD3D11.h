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

#include "SwapChain.h"

namespace nri
{
	struct NRI_D3D11_API SwapChainD3D11 : public SwapChain
	{
		SwapChainD3D11(DeviceD3D11& device);
		~SwapChainD3D11();

		inline DeviceD3D11& GetDevice() const
		{
			return m_Device;
		}

		Result Create(const VersionedDevice& device, const SwapChainDesc& swapChainDesc);

		//======================================================================================================================
		// NRI
		//======================================================================================================================
		virtual void SetDebugName(const char* name) override;
		virtual Texture* const* GetTextures(uint32_t& textureNum, Format& format) const override;
		virtual uint32_t AcquireNextTexture(QueueSemaphore& textureReadyForRender) override;
		virtual Result Present(QueueSemaphore& textureReadyForPresent) override;
		virtual Result SetHdrMetadata(const HdrMetadata& hdrMetadata) override;

	private:
		VersionedSwapchain m_SwapChain;
		Vector<TextureD3D11> m_RenderTargets;
		Vector<Texture*> m_RenderTargetPointers;
		SwapChainDesc m_SwapChainDesc = {};
		Format m_Format = Format::UNKNOWN;
		bool m_IsTearingAllowed = false;
		bool m_IsFullscreenEnabled = false;
		DeviceD3D11& m_Device;
	};
}
