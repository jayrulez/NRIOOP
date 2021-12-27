#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API CommandAllocator {
	public:
		virtual ~CommandAllocator() {}
		virtual void SetDebugName(const char* name) = 0;
		virtual Result CreateCommandBuffer(CommandBuffer*& commandBuffer) = 0;
		virtual void Reset() = 0;
	};
}