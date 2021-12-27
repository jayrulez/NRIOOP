/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#include <cstdarg>

#if _WIN32
#include <windows.h>
#else
#include <csignal>
#endif

#include "BaseDLL.h"
#include "Base.h"
#include "Lock.h"

constexpr std::array<const char*, uint32_t(nri::Message::TYPE_ERROR) + 1> MESSAGE_TYPE_NAME =
{
    "INFO",
    "WARNING",
    "ERROR"
};

constexpr std::array<const char*, uint32_t(nri::GraphicsAPI::VULKAN) + 1> GRAPHICS_API_NAME =
{
    "D3D11",
    "D3D12",
    "VULKAN"
};

Logger::Logger(nri::GraphicsAPI graphicsAPI, const nri::CallbackInterface& callbackInterface) :
    m_GraphicsAPI(graphicsAPI),
    m_CallbackInterface(callbackInterface)
{
}

void Logger::ReportMessage(nri::Message message, const char* format, ...) const
{
    const char* messageTypeName = MESSAGE_TYPE_NAME[(size_t)message];
    const char* graphicsAPIName = GRAPHICS_API_NAME[(size_t)m_GraphicsAPI];

    char buffer[4096];
    int written = snprintf(buffer, GetCountOf(buffer), "[nri(%s)::%s] -- ", graphicsAPIName, messageTypeName);

    va_list	argptr;
    va_start(argptr, format);
    written += vsnprintf(buffer + written, GetCountOf(buffer) - written, format, argptr);
    va_end(argptr);

    const int end = std::min(written, (int)GetCountOf(buffer) - 2);
    buffer[end] = '\n';
    buffer[end + 1] = '\0';

    if (m_CallbackInterface.MessageCallback != nullptr)
        m_CallbackInterface.MessageCallback(m_CallbackInterface.userArg, buffer, message);

    if (message == nri::Message::TYPE_ERROR && m_CallbackInterface.AbortExecution != nullptr)
        m_CallbackInterface.AbortExecution(m_CallbackInterface.userArg);
}

void ConvertCharToWchar(const char* in, wchar_t* out, size_t outLength)
{
    if (outLength == 0)
        return;

    for (size_t i = 0; i < outLength - 1 && *in; i++)
        *out++ = *in++;

    *out = 0;
}

void ConvertWcharToChar(const wchar_t* in, char* out, size_t outLength)
{
    if (outLength == 0)
        return;

    for (size_t i = 0; i < outLength - 1 && *in; i++)
        *out++ = char(*in++);

    *out = 0;
}

#if defined(_WIN32)

#include <winerror.h>

nri::Result GetResultFromHRESULT(long result)
{
    if (SUCCEEDED(result))
        return nri::Result::SUCCESS;

    if (result == E_INVALIDARG || result == E_POINTER || result == E_HANDLE)
        return nri::Result::INVALID_ARGUMENT;

    if (result == DXGI_ERROR_UNSUPPORTED)
        return nri::Result::UNSUPPORTED;

    if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
        return nri::Result::DEVICE_LOST;

    if (result == E_OUTOFMEMORY)
        return nri::Result::OUT_OF_MEMORY;

    return nri::Result::FAILURE;
}
#endif

constexpr std::array<uint32_t, (size_t)nri::Format::MAX_NUM> TEXEL_BLOCK_WIDTH = {
    0, // UNKNOWN

    1, // R8_UNORM
    1, // R8_SNORM
    1, // R8_UINT
    1, // R8_SINT

    1, // RG8_UNORM
    1, // RG8_SNORM
    1, // RG8_UINT
    1, // RG8_SINT

    1, // BGRA8_UNORM
    1, // BGRA8_SRGB

    1, // RGBA8_UNORM
    1, // RGBA8_SNORM
    1, // RGBA8_UINT
    1, // RGBA8_SINT
    1, // RGBA8_SRGB

    1, // R16_UNORM
    1, // R16_SNORM
    1, // R16_UINT
    1, // R16_SINT
    1, // R16_SFLOAT

    1, // RG16_UNORM
    1, // RG16_SNORM
    1, // RG16_UINT
    1, // RG16_SINT
    1, // RG16_SFLOAT

    1, // RGBA16_UNORM
    1, // RGBA16_SNORM
    1, // RGBA16_UINT
    1, // RGBA16_SINT
    1, // RGBA16_SFLOAT

    1, // R32_UINT
    1, // R32_SINT
    1, // R32_SFLOAT

    1, // RG32_UINT
    1, // RG32_SINT
    1, // RG32_SFLOAT

    1, // RGB32_UINT
    1, // RGB32_SINT
    1, // RGB32_SFLOAT

    1, // RGBA32_UINT
    1, // RGBA32_SINT
    1, // RGBA32_SFLOAT

    1, // R10_G10_B10_A2_UNORM
    1, // R10_G10_B10_A2_UINT
    1, // R11_G11_B10_UFLOAT
    1, // R9_G9_B9_E5_UFLOAT

    4, // BC1_RGBA_UNORM
    4, // BC1_RGBA_SRGB
    4, // BC2_RGBA_UNORM
    4, // BC2_RGBA_SRGB
    4, // BC3_RGBA_UNORM
    4, // BC3_RGBA_SRGB
    4, // BC4_R_UNORM
    4, // BC4_R_SNORM
    4, // BC5_RG_UNORM
    4, // BC5_RG_SNORM
    4, // BC6H_RGB_UFLOAT
    4, // BC6H_RGB_SFLOAT
    4, // BC7_RGBA_UNORM
    4, // BC7_RGBA_SRGB

    // DEPTH_STENCIL_ATTACHMENT views
    1, // D16_UNORM
    1, // D24_UNORM_S8_UINT
    1, // D32_SFLOAT
    1, // D32_SFLOAT_S8_UINT_X24

    // Depth-stencil specific SHADER_RESOURCE views
    0, // R24_UNORM_X8
    0, // X24_R8_UINT
    0, // X32_R8_UINT_X24
    0, // R32_SFLOAT_X8_X24
};

uint32_t GetTexelBlockWidth(nri::Format format)
{
    return TEXEL_BLOCK_WIDTH[(size_t)format];
}

constexpr std::array<uint32_t, (size_t)nri::Format::MAX_NUM> TEXEL_BLOCK_SIZE = {
    1, // UNKNOWN

    1, // R8_UNORM
    1, // R8_SNORM
    1, // R8_UINT
    1, // R8_SINT

    2, // RG8_UNORM
    2, // RG8_SNORM
    2, // RG8_UINT
    2, // RG8_SINT

    4, // BGRA8_UNORM
    4, // BGRA8_SRGB

    4, // RGBA8_UNORM
    4, // RGBA8_SNORM
    4, // RGBA8_UINT
    4, // RGBA8_SINT
    4, // RGBA8_SRGB

    2, // R16_UNORM
    2, // R16_SNORM
    2, // R16_UINT
    2, // R16_SINT
    2, // R16_SFLOAT

    4, // RG16_UNORM
    4, // RG16_SNORM
    4, // RG16_UINT
    4, // RG16_SINT
    4, // RG16_SFLOAT

    8, // RGBA16_UNORM
    8, // RGBA16_SNORM
    8, // RGBA16_UINT
    8, // RGBA16_SINT
    8, // RGBA16_SFLOAT

    4, // R32_UINT
    4, // R32_SINT
    4, // R32_SFLOAT

    8, // RG32_UINT
    8, // RG32_SINT
    8, // RG32_SFLOAT

    12, // RGB32_UINT
    12, // RGB32_SINT
    12, // RGB32_SFLOAT

    16, // RGBA32_UINT
    16, // RGBA32_SINT
    16, // RGBA32_SFLOAT

    4, // R10_G10_B10_A2_UNORM
    4, // R10_G10_B10_A2_UINT
    4, // R11_G11_B10_UFLOAT
    4, // R9_G9_B9_E5_UFLOAT

    8, // BC1_RGBA_UNORM
    8, // BC1_RGBA_SRGB
    16, // BC2_RGBA_UNORM
    16, // BC2_RGBA_SRGB
    16, // BC3_RGBA_UNORM
    16, // BC3_RGBA_SRGB
    8, // BC4_R_UNORM
    8, // BC4_R_SNORM
    16, // BC5_RG_UNORM
    16, // BC5_RG_SNORM
    16, // BC6H_RGB_UFLOAT
    16, // BC6H_RGB_SFLOAT
    16, // BC7_RGBA_UNORM
    16, // BC7_RGBA_SRGB

    // DEPTH_STENCIL_ATTACHMENT views
    2, // D16_UNORM
    4, // D24_UNORM_S8_UINT
    4, // D32_SFLOAT
    8, // D32_SFLOAT_S8_UINT_X24

    // Depth-stencil specific SHADER_RESOURCE views
    0, // R24_UNORM_X8
    0, // X24_R8_UINT
    0, // X32_R8_UINT_X24
    0, // R32_SFLOAT_X8_X24
};

uint32_t GetTexelBlockSize(nri::Format format)
{
    return TEXEL_BLOCK_SIZE[(size_t)format];
}



static void MessageCallback(void* userArg, const char* message, nri::Message messageType)
{
    MaybeUnused(userArg);
    MaybeUnused(messageType);

    fprintf(stderr, "%s", message);
#if _WIN32
    OutputDebugStringA(message);
#endif
}

static void AbortExecution(void* userArg)
{
    MaybeUnused(userArg);

#if _WIN32
    DebugBreak();
#else
    raise(SIGTRAP);
#endif
}

void CheckAndSetDefaultCallbacks(nri::CallbackInterface& callbackInterface)
{
    if (callbackInterface.MessageCallback == nullptr)
        callbackInterface.MessageCallback = MessageCallback;

    if (callbackInterface.AbortExecution == nullptr)
        callbackInterface.AbortExecution = AbortExecution;
}