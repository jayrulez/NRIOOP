#pragma once

#include "BaseDLL.h"

struct NRI_BASE_API HelperWaitIdle
{
    HelperWaitIdle(nri::Device& device, nri::CommandQueue& commandQueue);
    ~HelperWaitIdle();

    nri::Result WaitIdle();

private:
    nri::Device& m_Device;
    nri::CommandQueue& m_CommandQueue;
    nri::DeviceSemaphore* m_DeviceSemaphore = nullptr;
};