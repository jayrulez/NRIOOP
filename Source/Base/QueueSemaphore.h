#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API QueueSemaphore {
	public:
		virtual ~QueueSemaphore() {}
		virtual void SetDebugName(const char* name) = 0;
	};
}