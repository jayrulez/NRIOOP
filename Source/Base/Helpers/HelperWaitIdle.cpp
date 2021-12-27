#include "BaseDLL.h"
#include "Helpers/HelperWaitIdle.h"
#include "Device.h"
#include "CommandQueue.h"

using namespace nri;

HelperWaitIdle::HelperWaitIdle(Device& device, CommandQueue& commandQueue) :
    m_Device(device),
    m_CommandQueue(commandQueue)
{
    device.CreateDeviceSemaphore(false, m_DeviceSemaphore);
}

HelperWaitIdle::~HelperWaitIdle()
{
    if (m_DeviceSemaphore != nullptr)
        m_Device.DestroyDeviceSemaphore(*m_DeviceSemaphore);
    m_DeviceSemaphore = nullptr;
}

Result HelperWaitIdle::WaitIdle()
{
    if (m_DeviceSemaphore == nullptr)
        return Result::FAILURE;

    const uint32_t physicalDeviceNum = m_Device.GetDesc().phyiscalDeviceGroupSize;

    for (uint32_t i = 0; i < physicalDeviceNum; i++)
    {
        WorkSubmissionDesc workSubmissionDesc = {};
        workSubmissionDesc.physicalDeviceIndex = i;
        m_CommandQueue.Submit(workSubmissionDesc, m_DeviceSemaphore);
        m_CommandQueue.Wait(*m_DeviceSemaphore);
    }

    return Result::SUCCESS;
}
