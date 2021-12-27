#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API AccelerationStructure
	{
	public:
		virtual ~AccelerationStructure() {}

		virtual void SetDebugName(const char* name) = 0;
		virtual Result CreateDescriptor(uint32_t physicalDeviceMask, Descriptor*& descriptor) const = 0;
		virtual void GetMemoryInfo(MemoryDesc& memoryDesc) const = 0;
		virtual uint64_t GetUpdateScratchBufferSize() const = 0;
		virtual uint64_t GetBuildScratchBufferSize() const = 0;
		virtual uint64_t GetHandle(uint32_t physicalDeviceIndex) const = 0;
	};
}