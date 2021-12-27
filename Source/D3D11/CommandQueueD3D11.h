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
#include "CommandQueue.h"

namespace nri
{
    struct NRI_D3D11_API CommandQueueD3D11 : public CommandQueue
    {
        CommandQueueD3D11(DeviceD3D11& device, const VersionedContext& immediateContext);
        ~CommandQueueD3D11();

        inline DeviceD3D11& GetDevice() const;

        //======================================================================================================================
        // NRI
        //======================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual void Submit(const WorkSubmissionDesc& workSubmissions, DeviceSemaphore* deviceSemaphore) override;
        virtual void Wait(DeviceSemaphore& deviceSemaphore) override;

        virtual Result ChangeResourceStates(const TransitionBarrierDesc& transitionBarriers) override;
        virtual Result UploadData(const TextureUploadDesc* textureUploadDescs, uint32_t textureUploadDescNum,
            const BufferUploadDesc* bufferUploadDescs, uint32_t bufferUploadDescNum) override;
        virtual Result WaitForIdle() override;

    private:
        const VersionedContext& m_ImmediateContext;
        DeviceD3D11& m_Device;
    };
}
