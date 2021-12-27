#pragma once

#include <cstddef>

#include "D3DCommonDLL.h"

namespace nri {
    NRI_D3D_COMMON_API Format GetFormatDXGI(uint32_t dxgiFormat);
}

NRI_D3D_COMMON_API nri::Format GetFormat(uint32_t dxgiFormat);