/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

// Documentation: https://docs.google.com/document/d/1OidQtZnm-3grhua7Oy1WKDUzh8ExTdbO83VTAWS_nNM/

#pragma once



#include "BaseDLL.h"



// SharedExternal

#include <cstddef>
#include <array>
#include <atomic>
#include <type_traits>
#include <limits>
#include <cassert>
#include <cstring>
#include <map>

#include "StdAllocator.h"

#include "Helpers/HelperWaitIdle.h"
#include "Helpers/HelperDeviceMemoryAllocator.h"
#include "Helpers/HelperDataUpload.h"
#include "Helpers/HelperResourceStateChange.h"

constexpr uint32_t PHYSICAL_DEVICE_GROUP_MAX_SIZE = 4;
constexpr uint32_t COMMAND_QUEUE_TYPE_NUM = (uint32_t)nri::CommandQueueType::MAX_NUM;

NRI_BASE_API void CheckAndSetDefaultCallbacks(nri::CallbackInterface& callbackInterface);

struct NRI_BASE_API Logger
{
    Logger(nri::GraphicsAPI graphicsAPI, const nri::CallbackInterface& callbackInterface);

    void ReportMessage(nri::Message message, const char* format, ...) const;

private:
    nri::GraphicsAPI m_GraphicsAPI;
    nri::CallbackInterface m_CallbackInterface;
};



//==============================================================================================================================

#define RETURN_ON_BAD_HRESULT(log, hresult, format, ...) \
    if ( FAILED(hresult) ) \
    { \
        (log).ReportMessage(nri::Message::TYPE_ERROR, format, ##__VA_ARGS__); \
        return GetResultFromHRESULT(hresult); \
    }

#define RETURN_ON_FAILURE(log, condition, returnCode, format, ...) \
    if ( !(condition) ) \
    { \
        (log).ReportMessage(nri::Message::TYPE_ERROR, format, ##__VA_ARGS__); \
        return returnCode; \
    }

#define REPORT_INFO(log, format, ...) \
    (log).ReportMessage(nri::Message::TYPE_INFO, format, ##__VA_ARGS__)

#define REPORT_WARNING(log, format, ...) \
    (log).ReportMessage(nri::Message::TYPE_WARNING, format, ##__VA_ARGS__)

#define REPORT_ERROR(log, format, ...) \
    (log).ReportMessage(nri::Message::TYPE_ERROR, format, ##__VA_ARGS__)

#if _DEBUG
#define CHECK(log, condition, format, ...) \
        if ( !(condition) ) \
            (log).ReportMessage(nri::Message::TYPE_ERROR, format, ##__VA_ARGS__);
#else
#define CHECK(log, condition, format, ...) (void*)0;
#endif

constexpr void ReturnVoid() {}
template<typename T> constexpr void MaybeUnused([[maybe_unused]] const T& arg) {}

//==============================================================================================================================

NRI_BASE_API void ConvertCharToWchar(const char* in, wchar_t* out, size_t outLen);
NRI_BASE_API void ConvertWcharToChar(const wchar_t* in, char* out, size_t outLen);
NRI_BASE_API nri::Result GetResultFromHRESULT(long result);

inline nri::Vendor GetVendorFromID(uint32_t vendorID)
{
    switch (vendorID)
    {
    case 0x10DE: return nri::Vendor::NVIDIA;
    case 0x1002: return nri::Vendor::AMD;
    case 0x8086: return nri::Vendor::INTEL;
    }

    return nri::Vendor::UNKNOWN;
}

//==============================================================================================================================

// TODO: This code is Windows/D3D specific, so it's probably better to move it into a separate header
#if _WIN32

struct IUnknown;

// This struct acts as a smart pointer for IUnknown pointers making sure to call AddRef() and Release() as needed.
template<typename T>
struct ComPtr
{
    ComPtr(T* lComPtr = nullptr) : m_ComPtr(lComPtr)
    {
        static_assert(std::is_base_of<IUnknown, T>::value, "T needs to be IUnknown based");

        if (m_ComPtr)
            m_ComPtr->AddRef();
    }

    ComPtr(const ComPtr<T>& lComPtrObj)
    {
        static_assert(std::is_base_of<IUnknown, T>::value, "T needs to be IUnknown based");

        m_ComPtr = lComPtrObj.m_ComPtr;

        if (m_ComPtr)
            m_ComPtr->AddRef();
    }

    ComPtr(ComPtr<T>&& lComPtrObj)
    {
        m_ComPtr = lComPtrObj.m_ComPtr;
        lComPtrObj.m_ComPtr = nullptr;
    }

    T* operator=(T* lComPtr)
    {
        if (m_ComPtr)
            m_ComPtr->Release();

        m_ComPtr = lComPtr;

        if (m_ComPtr)
            m_ComPtr->AddRef();

        return m_ComPtr;
    }

    T* operator=(const ComPtr<T>& lComPtrObj)
    {
        if (m_ComPtr)
            m_ComPtr->Release();

        m_ComPtr = lComPtrObj.m_ComPtr;

        if (m_ComPtr)
            m_ComPtr->AddRef();

        return m_ComPtr;
    }

    ~ComPtr()
    {
        if (m_ComPtr)
        {
            m_ComPtr->Release();
            m_ComPtr = nullptr;
        }
    }

    operator T* () const
    {
        return m_ComPtr;
    }

    T* GetInterface() const
    {
        return m_ComPtr;
    }

    T& operator*() const
    {
        return *m_ComPtr;
    }

    T** operator&()
    {
        //The assert on operator& usually indicates a bug. Could be a potential memory leak.
        // If this really what is needed, however, use GetInterface() explicitly.
        assert(m_ComPtr == nullptr);
        return &m_ComPtr;
    }

    T* operator->() const
    {
        return m_ComPtr;
    }

    bool operator!() const
    {
        return (nullptr == m_ComPtr);
    }

    bool operator<(T* lComPtr) const
    {
        return m_ComPtr < lComPtr;
    }

    bool operator!=(T* lComPtr) const
    {
        return !operator==(lComPtr);
    }

    bool operator==(T* lComPtr) const
    {
        return m_ComPtr == lComPtr;
    }

protected:
    T* m_ComPtr;
};

constexpr nri::FormatSupportBits COMMON_SUPPORT =
nri::FormatSupportBits::TEXTURE |
nri::FormatSupportBits::STORAGE_TEXTURE |
nri::FormatSupportBits::BUFFER |
nri::FormatSupportBits::STORAGE_BUFFER |
nri::FormatSupportBits::COLOR_ATTACHMENT |
nri::FormatSupportBits::VERTEX_BUFFER;

constexpr nri::FormatSupportBits COMMON_SUPPORT_WITHOUT_VERTEX =
nri::FormatSupportBits::TEXTURE |
nri::FormatSupportBits::STORAGE_TEXTURE |
nri::FormatSupportBits::BUFFER |
nri::FormatSupportBits::STORAGE_BUFFER |
nri::FormatSupportBits::COLOR_ATTACHMENT;

constexpr nri::FormatSupportBits D3D_FORMAT_SUPPORT_TABLE[] = {
    nri::FormatSupportBits::UNSUPPORTED, // UNKNOWN,

    COMMON_SUPPORT_WITHOUT_VERTEX, // R8_UNORM,
    COMMON_SUPPORT_WITHOUT_VERTEX, // R8_SNORM,
    COMMON_SUPPORT_WITHOUT_VERTEX, // R8_UINT,
    COMMON_SUPPORT_WITHOUT_VERTEX, // R8_SINT,

    COMMON_SUPPORT_WITHOUT_VERTEX, // RG8_UNORM,
    COMMON_SUPPORT_WITHOUT_VERTEX, // RG8_SNORM,
    COMMON_SUPPORT_WITHOUT_VERTEX, // RG8_UINT,
    COMMON_SUPPORT_WITHOUT_VERTEX, // RG8_SINT,

    COMMON_SUPPORT_WITHOUT_VERTEX, // BGRA8_UNORM,
    COMMON_SUPPORT_WITHOUT_VERTEX, // BGRA8_SRGB,

    COMMON_SUPPORT_WITHOUT_VERTEX, // RGBA8_UNORM,
    COMMON_SUPPORT_WITHOUT_VERTEX, // RGBA8_SNORM,
    COMMON_SUPPORT_WITHOUT_VERTEX, // RGBA8_UINT,
    COMMON_SUPPORT_WITHOUT_VERTEX, // RGBA8_SINT,
    COMMON_SUPPORT_WITHOUT_VERTEX, // RGBA8_SRGB,

    COMMON_SUPPORT, // R16_UNORM,
    COMMON_SUPPORT, // R16_SNORM,
    COMMON_SUPPORT, // R16_UINT,
    COMMON_SUPPORT, // R16_SINT,
    COMMON_SUPPORT, // R16_SFLOAT,

    COMMON_SUPPORT, // RG16_UNORM,
    COMMON_SUPPORT, // RG16_SNORM,
    COMMON_SUPPORT, // RG16_UINT,
    COMMON_SUPPORT, // RG16_SINT,
    COMMON_SUPPORT, // RG16_SFLOAT,

    COMMON_SUPPORT, // RGBA16_UNORM,
    COMMON_SUPPORT, // RGBA16_SNORM,
    COMMON_SUPPORT, // RGBA16_UINT,
    COMMON_SUPPORT, // RGBA16_SINT,
    COMMON_SUPPORT, // RGBA16_SFLOAT,

    COMMON_SUPPORT, // R32_UINT,
    COMMON_SUPPORT, // R32_SINT,
    COMMON_SUPPORT, // R32_SFLOAT,

    COMMON_SUPPORT, // RG32_UINT,
    COMMON_SUPPORT, // RG32_SINT,
    COMMON_SUPPORT, // RG32_SFLOAT,

    COMMON_SUPPORT, // RGB32_UINT,
    COMMON_SUPPORT, // RGB32_SINT,
    COMMON_SUPPORT, // RGB32_SFLOAT,

    COMMON_SUPPORT, // RGBA32_UINT,
    COMMON_SUPPORT, // RGBA32_SINT,
    COMMON_SUPPORT, // RGBA32_SFLOAT,

    COMMON_SUPPORT_WITHOUT_VERTEX, // R10_G10_B10_A2_UNORM,
    COMMON_SUPPORT_WITHOUT_VERTEX, // R10_G10_B10_A2_UINT,
    COMMON_SUPPORT_WITHOUT_VERTEX, // R11_G11_B10_UFLOAT,
    COMMON_SUPPORT_WITHOUT_VERTEX, // R9_G9_B9_E5_UFLOAT,

    nri::FormatSupportBits::TEXTURE, // BC1_RGBA_UNORM,
    nri::FormatSupportBits::TEXTURE, // BC1_RGBA_SRGB,
    nri::FormatSupportBits::TEXTURE, // BC2_RGBA_UNORM,
    nri::FormatSupportBits::TEXTURE, // BC2_RGBA_SRGB,
    nri::FormatSupportBits::TEXTURE, // BC3_RGBA_UNORM,
    nri::FormatSupportBits::TEXTURE, // BC3_RGBA_SRGB,
    nri::FormatSupportBits::TEXTURE, // BC4_R_UNORM,
    nri::FormatSupportBits::TEXTURE, // BC4_R_SNORM,
    nri::FormatSupportBits::TEXTURE, // BC5_RG_UNORM,
    nri::FormatSupportBits::TEXTURE, // BC5_RG_SNORM,
    nri::FormatSupportBits::TEXTURE, // BC6H_RGB_UFLOAT,
    nri::FormatSupportBits::TEXTURE, // BC6H_RGB_SFLOAT,
    nri::FormatSupportBits::TEXTURE, // BC7_RGBA_UNORM,
    nri::FormatSupportBits::TEXTURE, // BC7_RGBA_SRGB,

    // DEPTH_STENCIL_ATTACHMENT views
    nri::FormatSupportBits::DEPTH_STENCIL_ATTACHMENT, // D16_UNORM,
    nri::FormatSupportBits::DEPTH_STENCIL_ATTACHMENT, // D24_UNORM_S8_UINT,
    nri::FormatSupportBits::DEPTH_STENCIL_ATTACHMENT, // D32_SFLOAT,
    nri::FormatSupportBits::DEPTH_STENCIL_ATTACHMENT, // D32_SFLOAT_S8_UINT_X24,

    // Depth-stencil specific SHADER_RESOURCE views
    nri::FormatSupportBits::TEXTURE, // R24_UNORM_X8,
    nri::FormatSupportBits::TEXTURE, // X24_R8_UINT,
    nri::FormatSupportBits::TEXTURE, // X32_R8_UINT_X24,
    nri::FormatSupportBits::TEXTURE, // R32_SFLOAT_X8_X24,

    // MAX_NUM
};

static_assert(GetCountOf(D3D_FORMAT_SUPPORT_TABLE) == (size_t)nri::Format::MAX_NUM, "some format is missing");

#endif

NRI_BASE_API uint32_t GetTexelBlockWidth(nri::Format format);
NRI_BASE_API uint32_t GetTexelBlockSize(nri::Format format);

constexpr uint32_t GetPhysicalDeviceGroupMask(uint32_t mask)
{
    return mask == nri::WHOLE_DEVICE_GROUP ? 0xff : mask;
}

//struct Library;
//Library* LoadSharedLibrary(const char* path);
//void* GetSharedLibraryFunction(Library& library, const char* name);
//void UnloadSharedLibrary(Library& library);
extern const char* VULKAN_LOADER_NAME;

// CLayer
namespace nri {
    struct CTextureDesc : TextureDesc
    {
        CTextureDesc() = default;
        CTextureDesc(const TextureDesc& textureDesc) : TextureDesc(textureDesc)
        {}

        static constexpr TextureUsageBits DEFAULT_USAGE_MASK = TextureUsageBits::SHADER_RESOURCE;

        static CTextureDesc Texture1D(Format format, uint16_t width, uint16_t mipNum = 1, uint16_t arraySize = 1, TextureUsageBits usageMask = DEFAULT_USAGE_MASK);
        static CTextureDesc Texture2D(Format format, uint16_t width, uint16_t height, uint16_t mipNum = 1, uint16_t arraySize = 1, TextureUsageBits usageMask = DEFAULT_USAGE_MASK, uint8_t sampleNum = 1);
        static CTextureDesc Texture3D(Format format, uint16_t width, uint16_t height, uint16_t depth, uint16_t mipNum = 1, TextureUsageBits usageMask = DEFAULT_USAGE_MASK);
    };

    inline CTextureDesc CTextureDesc::Texture1D(Format format, uint16_t width, uint16_t mipNum, uint16_t arraySize, TextureUsageBits usageMask)
    {
        TextureDesc textureDesc = {};
        textureDesc.type = TextureType::TEXTURE_1D;
        textureDesc.format = format;
        textureDesc.usageMask = usageMask;
        textureDesc.size[0] = width;
        textureDesc.size[1] = 1;
        textureDesc.size[2] = 1;
        textureDesc.mipNum = mipNum;
        textureDesc.arraySize = arraySize;
        textureDesc.sampleNum = 1;

        return CTextureDesc(textureDesc);
    }

    inline CTextureDesc CTextureDesc::Texture2D(Format format, uint16_t width, uint16_t height, uint16_t mipNum, uint16_t arraySize, TextureUsageBits usageMask, uint8_t sampleNum)
    {
        TextureDesc textureDesc = {};
        textureDesc.type = TextureType::TEXTURE_2D;
        textureDesc.format = format;
        textureDesc.usageMask = usageMask;
        textureDesc.size[0] = width;
        textureDesc.size[1] = height;
        textureDesc.size[2] = 1;
        textureDesc.mipNum = mipNum;
        textureDesc.arraySize = arraySize;
        textureDesc.sampleNum = sampleNum;

        return CTextureDesc(textureDesc);
    }

    inline CTextureDesc CTextureDesc::Texture3D(Format format, uint16_t width, uint16_t height, uint16_t depth, uint16_t mipNum, TextureUsageBits usageMask)
    {
        TextureDesc textureDesc = {};
        textureDesc.type = TextureType::TEXTURE_3D;
        textureDesc.format = format;
        textureDesc.usageMask = usageMask;
        textureDesc.size[0] = width;
        textureDesc.size[1] = height;
        textureDesc.size[2] = depth;
        textureDesc.mipNum = mipNum;
        textureDesc.arraySize = 1;
        textureDesc.sampleNum = 1;

        return CTextureDesc(textureDesc);
    }
}