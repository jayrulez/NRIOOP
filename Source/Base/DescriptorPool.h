#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API DescriptorPool {
	public:
		virtual ~DescriptorPool() {}
		virtual   void SetDebugName(const char* name) = 0;

		virtual  Result AllocateDescriptorSets(const PipelineLayout& pipelineLayout, uint32_t setIndex, DescriptorSet** const descriptorSets, uint32_t instanceNum, uint32_t physicalDeviceMask, uint32_t variableDescriptorNum) = 0;

		virtual  void Reset() = 0;
	};
}