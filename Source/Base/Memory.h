#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API Memory {
	public:
		virtual ~Memory() {}
		virtual void SetDebugName(const char* name) = 0;
	};
}