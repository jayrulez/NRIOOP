#pragma once

#include "DeviceManagerDLL.h"
#include "BaseDLL.h"

namespace nri
{
	NRI_DEVICE_MANAGER_API Result CreateDevice(const DeviceCreationDesc& deviceCreationDesc, Device*& device);
	NRI_DEVICE_MANAGER_API void DestroyDevice(Device& device);
}