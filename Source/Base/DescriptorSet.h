#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API DescriptorSet {
	public:
		virtual ~DescriptorSet() {}
		virtual  void SetDebugName(const char* name) = 0;
		virtual  void UpdateDescriptorRanges(uint32_t physicalDeviceMask, uint32_t rangeOffset, uint32_t rangeNum, const DescriptorRangeUpdateDesc* rangeUpdateDescs) = 0;
		virtual  void UpdateDynamicConstantBuffers(uint32_t physicalDeviceMask, uint32_t baseBuffer, uint32_t bufferNum, const Descriptor* const* descriptors) = 0;
		virtual  void Copy(const DescriptorSetCopyDesc& descriptorSetCopyDesc) = 0;
	};
}