/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include "D3D11DLL.h"

#include "DescriptorSet.h"

namespace nri
{
    struct OffsetNum
    {
        uint32_t descriptorOffset;
        uint32_t descriptorNum;
    };

    struct NRI_D3D11_API DescriptorSetD3D11 : public DescriptorSet
    {
        DescriptorSetD3D11(DeviceD3D11& device);

        inline const DescriptorD3D11* GetDescriptor(uint32_t i) const
        { return m_Descriptors[i]; }

        uint32_t Initialize(const PipelineLayoutD3D11& pipelineLayout, uint32_t setIndex, const DescriptorD3D11** descriptors);

        //======================================================================================================================
        // NRI
        //======================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual void UpdateDescriptorRanges(uint32_t physicalDeviceMask, uint32_t rangeOffset, uint32_t rangeNum, const DescriptorRangeUpdateDesc* rangeUpdateDescs) override;
        virtual void UpdateDynamicConstantBuffers(uint32_t physicalDeviceMask, uint32_t baseBuffer, uint32_t bufferNum, const Descriptor* const* descriptors) override;
        virtual void Copy(const DescriptorSetCopyDesc& descriptorSetCopyDesc) override;

    private:
        const DescriptorD3D11** m_Descriptors = nullptr;
        Vector<OffsetNum> m_Ranges;
        Vector<OffsetNum> m_DynamicConstantBuffers;
    };
}
