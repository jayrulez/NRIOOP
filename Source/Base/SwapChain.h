#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API SwapChain {
	public:
		virtual ~SwapChain() {}
		virtual void SetDebugName(const char* name) = 0;

		virtual Texture* const* GetTextures(uint32_t& textureNum, Format& format) const = 0;
		virtual uint32_t AcquireNextTexture(QueueSemaphore& textureReadyForRender) = 0;
		virtual Result Present(QueueSemaphore& textureReadyForPresent) = 0;
		virtual Result SetHdrMetadata(const HdrMetadata& hdrMetadata) = 0;
	};
}