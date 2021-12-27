/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include <vector>
#include <array>
#include "FrameworkDLL.h"

#include "CommandBuffer.h"

namespace nri
{
    struct PhysicalDeviceGroup;
}

namespace helper
{
    template<typename T, typename A> constexpr T GetAlignedSize(const T& size, A alignment)
    {
        return T(((size + alignment - 1) / alignment) * alignment);
    }

    template <typename T, uint32_t N> constexpr uint32_t GetCountOf(T const (&)[N])
    {
        return N;
    }

    template <typename T> constexpr uint32_t GetCountOf(const std::vector<T>& v)
    {
        return (uint32_t)v.size();
    }

    template <typename T, size_t N> constexpr uint32_t GetCountOf(const std::array<T, N>& v)
    {
        return (uint32_t)v.size();
    }

    template<typename T, typename U> constexpr uint32_t GetOffsetOf(U T::* member)
    {
        return (uint32_t)((char*)&((T*)nullptr->*member) - (char*)nullptr);
    }

    template<typename T> constexpr size_t GetByteSizeOf(const std::vector<T>& v)
    {
        return v.size() * sizeof(decltype(v.back()));
    }

    struct Annotation
    {
        nri::CommandBuffer& m_CommandBuffer;

        inline Annotation(nri::CommandBuffer& commandBuffer, const char* name) :
            m_CommandBuffer(commandBuffer)
        {
            m_CommandBuffer.BeginAnnotation(name);
        }

        inline ~Annotation()
        {
            m_CommandBuffer.EndAnnotation();
        }
    };

    NRI_FRAMEWORK_API bool FindPhysicalDeviceGroup(nri::PhysicalDeviceGroup& physicalDeviceGroup);
}