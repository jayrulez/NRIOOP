/*
Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#include "D3D12DLL.h"

#include "CommandQueue.h"

struct ID3D12Device;
struct ID3D12CommandQueue;
enum D3D12_COMMAND_LIST_TYPE;

namespace nri
{
    struct   NRI_D3D12_API CommandQueueD3D12 : public CommandQueue
    {
        CommandQueueD3D12(DeviceD3D12& device);
        ~CommandQueueD3D12();

        operator ID3D12CommandQueue*() const;

        DeviceD3D12& GetDevice() const;

        Result Create(CommandQueueType queueType);
        Result Create(ID3D12CommandQueue* commandQueue);

        D3D12_COMMAND_LIST_TYPE GetType() const;

        //================================================================================================================
        // NRI
        //================================================================================================================
        virtual void SetDebugName(const char* name) override;
        virtual void Submit(const WorkSubmissionDesc& workSubmissions, DeviceSemaphore* deviceSemaphore) override;
        virtual void Wait(DeviceSemaphore& deviceSemaphore) override;

        virtual Result ChangeResourceStates(const TransitionBarrierDesc& transitionBarriers) override;
        virtual Result UploadData(const TextureUploadDesc* textureUploadDescs, uint32_t textureUploadDescNum,
            const BufferUploadDesc* bufferUploadDescs, uint32_t bufferUploadDescNum) override;
        virtual Result WaitForIdle() override;

    private:
        DeviceD3D12& m_Device;
        ComPtr<ID3D12CommandQueue> m_CommandQueue;
        D3D12_COMMAND_LIST_TYPE m_CommandListType = D3D12_COMMAND_LIST_TYPE(-1);
    };

    inline CommandQueueD3D12::CommandQueueD3D12(DeviceD3D12& device)
        : m_Device(device)
    {}

    inline CommandQueueD3D12::~CommandQueueD3D12()
    {}

    inline CommandQueueD3D12::operator ID3D12CommandQueue*() const
    {
        return m_CommandQueue.GetInterface();
    }

    inline DeviceD3D12& CommandQueueD3D12::GetDevice() const
    {
        return m_Device;
    }
}
