#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API QueryPool {
	public:
		virtual ~QueryPool() {}
		virtual void SetDebugName(const char* name) = 0;
		virtual uint32_t GetQuerySize() const = 0;
	};
}