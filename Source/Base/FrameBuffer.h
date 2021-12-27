#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API FrameBuffer {
	public:
		virtual ~FrameBuffer() {}
		virtual void SetDebugName(const char* name) = 0;
	};
}