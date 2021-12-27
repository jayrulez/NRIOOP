#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API Pipeline {
	public:
		virtual ~Pipeline() {}
		virtual void SetDebugName(const char* name) = 0;
		virtual  Result WriteShaderGroupIdentifiers(uint32_t baseShaderGroupIndex, uint32_t shaderGroupNum, void* buffer) const = 0;
	};
}