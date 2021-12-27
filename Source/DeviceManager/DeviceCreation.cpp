/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#include "DeviceCreation.h"
#include "DeviceVal.h"
#include "Device.h"

using namespace nri;

template< typename T >
Result FinalizeDeviceCreation(const T& deviceCreationDesc, Device& deviceImpl, Device*& device)
{
    if (deviceCreationDesc.enableNRIValidation)
    {
        Device* deviceVal = (Device*)CreateDeviceValidation(deviceCreationDesc, deviceImpl);
        if (deviceVal == nullptr)
        {
            deviceImpl.Destroy();
            nri::DestroyDevice((Device&)deviceImpl);
            return Result::FAILURE;
        }

        device = deviceVal;
    }
    else
    {
        device = (Device*)&deviceImpl;
    }

    return Result::SUCCESS;
}

#if (NRI_USE_D3D11 == 1)
#include "D3D12DLL.h"
#include "DeviceD3D11.h"


Result CreateDeviceFromD3D11Device(const DeviceCreationD3D11Desc& deviceDesc, Device*& device);

Result CreateDeviceFromD3D11Device(const DeviceCreationD3D11Desc& deviceCreationD3D11Desc, Device*& device)
{
    DeviceCreationDesc deviceCreationDesc = {};
    deviceCreationDesc.callbackInterface = deviceCreationD3D11Desc.callbackInterface;
    deviceCreationDesc.memoryAllocatorInterface = deviceCreationD3D11Desc.memoryAllocatorInterface;
    deviceCreationDesc.graphicsAPI = GraphicsAPI::D3D11;
    deviceCreationDesc.enableNRIValidation = deviceCreationD3D11Desc.enableNRIValidation;
    deviceCreationDesc.enableAPIValidation = deviceCreationD3D11Desc.enableAPIValidation;

    CheckAndSetDefaultCallbacks(deviceCreationDesc.callbackInterface);
    CheckAndSetDefaultAllocator(deviceCreationDesc.memoryAllocatorInterface);

    Result result = Result::UNSUPPORTED;
    Device* deviceImpl = nullptr;

    result = CreateDeviceD3D11(deviceCreationD3D11Desc, deviceImpl);

    if (result != Result::SUCCESS)
        return result;

    return FinalizeDeviceCreation(deviceCreationDesc, *deviceImpl, device);
}
#endif

#if (NRI_USE_D3D12 == 1)
#include "D3D12DLL.h"
#include "DeviceD3D12.h"

Result CreateDeviceFromD3D12Device(const DeviceCreationD3D12Desc& deviceDesc, Device*& device);

Result CreateDeviceFromD3D12Device(const DeviceCreationD3D12Desc& deviceCreationD3D12Desc, Device*& device)
{
    DeviceCreationDesc deviceCreationDesc = {};
    deviceCreationDesc.callbackInterface = deviceCreationD3D12Desc.callbackInterface;
    deviceCreationDesc.memoryAllocatorInterface = deviceCreationD3D12Desc.memoryAllocatorInterface;
    deviceCreationDesc.graphicsAPI = GraphicsAPI::D3D12;
    deviceCreationDesc.enableNRIValidation = deviceCreationD3D12Desc.enableNRIValidation;
    deviceCreationDesc.enableAPIValidation = deviceCreationD3D12Desc.enableAPIValidation;

    CheckAndSetDefaultCallbacks(deviceCreationDesc.callbackInterface);
    CheckAndSetDefaultAllocator(deviceCreationDesc.memoryAllocatorInterface);

    DeviceCreationD3D12Desc tempDeviceCreationD3D12Desc = deviceCreationD3D12Desc;

    Result result = Result::UNSUPPORTED;
    Device* deviceImpl = nullptr;

    CheckAndSetDefaultCallbacks(tempDeviceCreationD3D12Desc.callbackInterface);
    CheckAndSetDefaultAllocator(tempDeviceCreationD3D12Desc.memoryAllocatorInterface);

#if (NRI_USE_D3D12 == 1)
    result = CreateDeviceD3D12(tempDeviceCreationD3D12Desc, deviceImpl);
#endif

    if (result != Result::SUCCESS)
        return result;

    return FinalizeDeviceCreation(deviceCreationDesc, *deviceImpl, device);
}
#endif

#if (NRI_USE_VULKAN == 1)
#include "DeviceVK.h"
#include "VKDLL.h"

Result CreateDeviceFromVkDevice(const DeviceCreationVulkanDesc& deviceDesc, Device*& device);


Result CreateDeviceFromVkDevice(const DeviceCreationVulkanDesc& deviceCreationVulkanDesc, Device*& device)
{
    DeviceCreationDesc deviceCreationDesc = {};
    deviceCreationDesc.callbackInterface = deviceCreationVulkanDesc.callbackInterface;
    deviceCreationDesc.memoryAllocatorInterface = deviceCreationVulkanDesc.memoryAllocatorInterface;
    deviceCreationDesc.graphicsAPI = GraphicsAPI::VULKAN;
    deviceCreationDesc.enableNRIValidation = deviceCreationVulkanDesc.enableNRIValidation;
    deviceCreationDesc.enableAPIValidation = deviceCreationVulkanDesc.enableAPIValidation;

    CheckAndSetDefaultCallbacks(deviceCreationDesc.callbackInterface);
    CheckAndSetDefaultAllocator(deviceCreationDesc.memoryAllocatorInterface);

    DeviceCreationVulkanDesc tempDeviceCreationVulkanDesc = deviceCreationVulkanDesc;

    CheckAndSetDefaultCallbacks(tempDeviceCreationVulkanDesc.callbackInterface);
    CheckAndSetDefaultAllocator(tempDeviceCreationVulkanDesc.memoryAllocatorInterface);

    Result result = Result::UNSUPPORTED;
    Device* deviceImpl = nullptr;

    result = CreateDeviceVK(tempDeviceCreationVulkanDesc, deviceImpl);

    if (result != Result::SUCCESS)
        return result;

    return FinalizeDeviceCreation(deviceCreationDesc, *deviceImpl, device);
}
#endif

using namespace nri;


//Format GetFormatVK(uint32_t vkFormat);

constexpr uint64_t Hash( const char* name )
{
    return *name != 0 ? *name ^ ( 33 * Hash(name + 1) ) : 5381;
}

Result nri::CreateDevice(const DeviceCreationDesc& deviceCreationDesc, Device*& device)
{
    Result result = Result::UNSUPPORTED;
    Device* deviceImpl = nullptr;

    DeviceCreationDesc modifiedDeviceCreationDesc = deviceCreationDesc;
    CheckAndSetDefaultCallbacks(modifiedDeviceCreationDesc.callbackInterface);
    CheckAndSetDefaultAllocator(modifiedDeviceCreationDesc.memoryAllocatorInterface);

    #if (NRI_USE_D3D11 == 1)
        if (modifiedDeviceCreationDesc.graphicsAPI == GraphicsAPI::D3D11)
            result = CreateDeviceD3D11(modifiedDeviceCreationDesc, deviceImpl);
    #endif

    #if (NRI_USE_D3D12 == 1)
        if (modifiedDeviceCreationDesc.graphicsAPI == GraphicsAPI::D3D12)
            result = CreateDeviceD3D12(modifiedDeviceCreationDesc, deviceImpl);
    #endif

    //#if (NRI_USE_VULKAN == 1)
    //    if (modifiedDeviceCreationDesc.graphicsAPI == GraphicsAPI::VULKAN)
    //        result = CreateDeviceVK(modifiedDeviceCreationDesc, deviceImpl);
    //#endif

    if (result != Result::SUCCESS)
        return result;

    return FinalizeDeviceCreation(modifiedDeviceCreationDesc, *deviceImpl, device);
}

void nri::DestroyDevice(Device& device)
{
    ((Device&)device).Destroy();
}
