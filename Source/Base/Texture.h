#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API Texture {
	public:
		virtual ~Texture() {}
		virtual void SetDebugName(const char* name) = 0;
		virtual void GetMemoryInfo(MemoryLocation memoryLocation, MemoryDesc& memoryDesc) const = 0;
	};
}