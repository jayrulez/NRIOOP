/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include "VKDLL.h"

#include "DeviceSemaphore.h"

namespace nri
{
    struct NRI_VK_API DeviceSemaphoreVK : public DeviceSemaphore
    {
        DeviceSemaphoreVK(DeviceVK& device);
        ~DeviceSemaphoreVK();

        operator VkFence() const;
        DeviceVK& GetDevice() const;
        Result Create(bool signaled);
        Result Create(void* vkFence);

        virtual void SetDebugName(const char* name) override;

    private:
        VkFence m_Handle = VK_NULL_HANDLE;
        DeviceVK& m_Device;
        bool m_OwnsNativeObjects = false;
    };

    inline DeviceSemaphoreVK::DeviceSemaphoreVK(DeviceVK& device) :
        m_Device(device)
    {
    }

    inline DeviceSemaphoreVK::operator VkFence() const
    {
        return m_Handle;
    }

    inline DeviceVK& DeviceSemaphoreVK::GetDevice() const
    {
        return m_Device;
    }
}