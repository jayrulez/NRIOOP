#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API DeviceSemaphore {
	public:
		virtual ~DeviceSemaphore() {}
		virtual void SetDebugName(const char* name) = 0;
	};
}