#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API Buffer {
	public:
		virtual ~Buffer() {}
		virtual void SetDebugName(const char* name) = 0;
		virtual void GetMemoryInfo(MemoryLocation memoryLocation, MemoryDesc& memoryDesc) const = 0;
		virtual void* Map(uint64_t offset, uint64_t size) = 0;
		virtual void Unmap() = 0;
	};
}