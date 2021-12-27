#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API Descriptor {
	public:
		virtual ~Descriptor() {}
		virtual  void SetDebugName(const char* name) = 0;
	};
}