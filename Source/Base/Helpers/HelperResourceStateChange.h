#pragma once

#include "BaseDLL.h"
#include "Helpers/HelperWaitIdle.h"

struct NRI_BASE_API HelperResourceStateChange
{
    HelperResourceStateChange(nri::Device& device, nri::CommandQueue& commandQueue);
    ~HelperResourceStateChange();

    nri::Result ChangeStates(const nri::TransitionBarrierDesc& transitionBarriers);

private:
    nri::Device& m_Device;
    nri::CommandQueue& m_CommandQueue;
    nri::CommandAllocator* m_CommandAllocator = nullptr;
    nri::CommandBuffer* m_CommandBuffer = nullptr;
    HelperWaitIdle m_HelperWaitIdle;
};