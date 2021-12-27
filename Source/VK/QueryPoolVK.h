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

#include "QueryPool.h"

namespace nri
{
    struct NRI_VK_API QueryPoolVK : public QueryPool
    {
        QueryPoolVK(DeviceVK& device);
        ~QueryPoolVK();

        VkQueryPool GetHandle(uint32_t physicalDeviceIndex) const;
        DeviceVK& GetDevice() const;
        uint32_t GetStride() const;
        VkQueryType GetType() const;
        Result Create(const QueryPoolDesc& queryPoolDesc);
        Result Create(const QueryPoolVulkanDesc& queryPoolDesc);

        virtual void SetDebugName(const char* name) override;
        virtual uint32_t GetQuerySize() const override;

    private:
        std::array<VkQueryPool, PHYSICAL_DEVICE_GROUP_MAX_SIZE> m_Handles = {};
        uint32_t m_Stride = 0;
        VkQueryType m_Type = (VkQueryType)0;
        DeviceVK& m_Device;
        bool m_OwnsNativeObjects = false;
    };

    inline QueryPoolVK::QueryPoolVK(DeviceVK& device) :
        m_Device(device)
    {
    }

    inline VkQueryPool QueryPoolVK::GetHandle(uint32_t physicalDeviceIndex) const
    {
        return m_Handles[physicalDeviceIndex];
    }

    inline DeviceVK& QueryPoolVK::GetDevice() const
    {
        return m_Device;
    }

    inline uint32_t QueryPoolVK::GetStride() const
    {
        return m_Stride;
    }

    inline VkQueryType QueryPoolVK::GetType() const
    {
        return m_Type;
    }
}