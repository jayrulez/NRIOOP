#include "Helpers.h"

#include <cstdarg>

#include <windows.h>
#include <winerror.h>
#include <array>

using namespace nri;

Format GetFormatDXGI(uint32_t dxgiFormat);

constexpr std::array<nri::Format, 100> DXGI_FORMAT_TABLE =
{
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_UNKNOWN = 0,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
    nri::Format::RGBA32_SFLOAT,                        // DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
    nri::Format::RGBA32_UINT,                          // DXGI_FORMAT_R32G32B32A32_UINT = 3,
    nri::Format::RGBA32_SINT,                          // DXGI_FORMAT_R32G32B32A32_SINT = 4,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R32G32B32_TYPELESS = 5,
    nri::Format::RGB32_SFLOAT,                         // DXGI_FORMAT_R32G32B32_FLOAT = 6,
    nri::Format::RGB32_UINT,                           // DXGI_FORMAT_R32G32B32_UINT = 7,
    nri::Format::RGB32_SINT,                           // DXGI_FORMAT_R32G32B32_SINT = 8,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
    nri::Format::RGBA16_SFLOAT,                        // DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
    nri::Format::RGBA16_UNORM,                         // DXGI_FORMAT_R16G16B16A16_UNORM = 11,
    nri::Format::RGBA16_UINT,                          // DXGI_FORMAT_R16G16B16A16_UINT = 12,
    nri::Format::RGBA16_SNORM,                         // DXGI_FORMAT_R16G16B16A16_SNORM = 13,
    nri::Format::RGBA16_SINT,                          // DXGI_FORMAT_R16G16B16A16_SINT = 14,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R32G32_TYPELESS = 15,
    nri::Format::RG32_SFLOAT,                          // DXGI_FORMAT_R32G32_FLOAT = 16,
    nri::Format::RG32_UINT,                            // DXGI_FORMAT_R32G32_UINT = 17,
    nri::Format::RGB32_SINT,                           // DXGI_FORMAT_R32G32_SINT = 18,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R32G8X24_TYPELESS = 19,
    nri::Format::D32_SFLOAT_S8_UINT_X24,               // DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
    nri::Format::R32_SFLOAT_X8_X24,                    // DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
    nri::Format::X32_R8_UINT_X24,                      // DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
    nri::Format::R10_G10_B10_A2_UNORM,                 // DXGI_FORMAT_R10G10B10A2_UNORM = 24,
    nri::Format::R10_G10_B10_A2_UINT,                  // DXGI_FORMAT_R10G10B10A2_UINT = 25,
    nri::Format::R11_G11_B10_UFLOAT,                   // DXGI_FORMAT_R11G11B10_FLOAT = 26,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
    nri::Format::RGBA8_UNORM,                          // DXGI_FORMAT_R8G8B8A8_UNORM = 28,
    nri::Format::RGBA8_SRGB,                           // DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
    nri::Format::RGBA8_UINT,                           // DXGI_FORMAT_R8G8B8A8_UINT = 30,
    nri::Format::RGBA8_SNORM,                          // DXGI_FORMAT_R8G8B8A8_SNORM = 31,
    nri::Format::RGBA8_SINT,                           // DXGI_FORMAT_R8G8B8A8_SINT = 32,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R16G16_TYPELESS = 33,
    nri::Format::RG16_SFLOAT,                          // DXGI_FORMAT_R16G16_FLOAT = 34,
    nri::Format::RG16_UNORM,                           // DXGI_FORMAT_R16G16_UNORM = 35,
    nri::Format::RG16_UINT,                            // DXGI_FORMAT_R16G16_UINT = 36,
    nri::Format::RG16_SNORM,                           // DXGI_FORMAT_R16G16_SNORM = 37,
    nri::Format::RG16_SINT,                            // DXGI_FORMAT_R16G16_SINT = 38,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R32_TYPELESS = 39,
    nri::Format::D32_SFLOAT,                           // DXGI_FORMAT_D32_FLOAT = 40,
    nri::Format::R32_SFLOAT,                           // DXGI_FORMAT_R32_FLOAT = 41,
    nri::Format::R32_UINT,                             // DXGI_FORMAT_R32_UINT = 42,
    nri::Format::R32_SINT,                             // DXGI_FORMAT_R32_SINT = 43,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R24G8_TYPELESS = 44,
    nri::Format::D24_UNORM_S8_UINT,                    // DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
    nri::Format::R24_UNORM_X8,                         // DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
    nri::Format::X24_R8_UINT,                          // DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R8G8_TYPELESS = 48,
    nri::Format::RG8_UNORM,                            // DXGI_FORMAT_R8G8_UNORM = 49,
    nri::Format::RG8_UINT,                             // DXGI_FORMAT_R8G8_UINT = 50,
    nri::Format::RG8_SNORM,                            // DXGI_FORMAT_R8G8_SNORM = 51,
    nri::Format::RG8_SINT,                             // DXGI_FORMAT_R8G8_SINT = 52,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R16_TYPELESS = 53,
    nri::Format::R16_SFLOAT,                           // DXGI_FORMAT_R16_FLOAT = 54,
    nri::Format::D16_UNORM,                            // DXGI_FORMAT_D16_UNORM = 55,
    nri::Format::R16_UNORM,                            // DXGI_FORMAT_R16_UNORM = 56,
    nri::Format::R16_UINT,                             // DXGI_FORMAT_R16_UINT = 57,
    nri::Format::R16_SNORM,                            // DXGI_FORMAT_R16_SNORM = 58,
    nri::Format::R16_SINT,                             // DXGI_FORMAT_R16_SINT = 59,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R8_TYPELESS = 60,
    nri::Format::R8_UNORM,                             // DXGI_FORMAT_R8_UNORM = 61,
    nri::Format::R8_UINT,                              // DXGI_FORMAT_R8_UINT = 62,
    nri::Format::R8_SNORM,                             // DXGI_FORMAT_R8_SNORM = 63,
    nri::Format::R8_SINT,                              // DXGI_FORMAT_R8_SINT = 64,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_A8_UNORM = 65,

    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R1_UNORM = 66,
    nri::Format::R9_G9_B9_E5_UFLOAT,                   // DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_BC1_TYPELESS = 70,
    nri::Format::BC1_RGBA_UNORM,                       // DXGI_FORMAT_BC1_UNORM = 71,
    nri::Format::BC1_RGBA_SRGB,                        // DXGI_FORMAT_BC1_UNORM_SRGB = 72,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_BC2_TYPELESS = 73,
    nri::Format::BC2_RGBA_UNORM,                       // DXGI_FORMAT_BC2_UNORM = 74,
    nri::Format::BC2_RGBA_SRGB,                        // DXGI_FORMAT_BC2_UNORM_SRGB = 75,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_BC3_TYPELESS = 76,
    nri::Format::BC3_RGBA_UNORM,                       // DXGI_FORMAT_BC3_UNORM = 77,
    nri::Format::BC3_RGBA_SRGB,                        // DXGI_FORMAT_BC3_UNORM_SRGB = 78,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_BC4_TYPELESS = 79,
    nri::Format::BC4_R_UNORM,                          // DXGI_FORMAT_BC4_UNORM = 80,
    nri::Format::BC4_R_SNORM,                          // DXGI_FORMAT_BC4_SNORM = 81,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_BC5_TYPELESS = 82,
    nri::Format::BC5_RG_UNORM,                         // DXGI_FORMAT_BC5_UNORM = 83,
    nri::Format::BC5_RG_SNORM,                         // DXGI_FORMAT_BC5_SNORM = 84,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_B5G6R5_UNORM = 85,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_B5G5R5A1_UNORM = 86,
    nri::Format::BGRA8_UNORM,                          // DXGI_FORMAT_B8G8R8A8_UNORM = 87,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_B8G8R8X8_UNORM = 88,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
    nri::Format::BGRA8_SRGB,                           // DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_BC6H_TYPELESS = 94,
    nri::Format::BC6H_RGB_UFLOAT,                      // DXGI_FORMAT_BC6H_UF16 = 95,
    nri::Format::BC6H_RGB_SFLOAT,                      // DXGI_FORMAT_BC6H_SF16 = 96,
    nri::Format::UNKNOWN,                              // DXGI_FORMAT_BC7_TYPELESS = 97,
    nri::Format::BC7_RGBA_UNORM,                       // DXGI_FORMAT_BC7_UNORM = 98,
    nri::Format::BC7_RGBA_SRGB,                        // DXGI_FORMAT_BC7_UNORM_SRGB = 99,
};

nri::Format GetFormat(uint32_t dxgiFormat)
{
    return DXGI_FORMAT_TABLE[dxgiFormat];
}

nri::Format GetFormatDXGI(uint32_t dxgiFormat)
{
    return GetFormat(dxgiFormat);
}

Format nri::GetFormatDXGI(uint32_t dxgiFormat)
{
#if (NRI_USE_D3D11 == 1 || NRI_USE_D3D12 == 1)
    return ::GetFormatDXGI(dxgiFormat);
#else
    return nri::Format::UNKNOWN;
#endif
}