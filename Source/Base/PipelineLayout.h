#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API PipelineLayout {
	public:

		virtual ~PipelineLayout() {}
		virtual void SetDebugName(const char* name) = 0;
	};
}