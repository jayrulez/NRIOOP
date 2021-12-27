#include "BaseDLL.h"
#include "Helpers/HelperResourceStateChange.h"
#include "Device.h"
#include "CommandAllocator.h"
#include "CommandBuffer.h"
#include "CommandQueue.h"

using namespace nri;

HelperResourceStateChange::HelperResourceStateChange(Device& device, CommandQueue& commandQueue) :
    m_Device(device),
    m_CommandQueue(commandQueue),
    m_HelperWaitIdle(device, commandQueue)
{
    if (m_Device.CreateCommandAllocator(commandQueue, WHOLE_DEVICE_GROUP, m_CommandAllocator) == Result::SUCCESS)
        (*m_CommandAllocator).CreateCommandBuffer(m_CommandBuffer);
}

HelperResourceStateChange::~HelperResourceStateChange()
{
    if (m_CommandBuffer != nullptr)
        m_Device.DestroyCommandBuffer(*m_CommandBuffer);
    m_CommandBuffer = nullptr;

    if (m_CommandAllocator != nullptr)
        m_Device.DestroyCommandAllocator(*m_CommandAllocator);
    m_CommandAllocator = nullptr;
}

Result HelperResourceStateChange::ChangeStates(const TransitionBarrierDesc& transitionBarriers)
{
    if (m_CommandBuffer == nullptr)
        return Result::FAILURE;

    const uint32_t physicalDeviceNum = m_Device.GetDesc().phyiscalDeviceGroupSize;

    for (uint32_t i = 0; i < physicalDeviceNum; i++)
    {
        (*m_CommandBuffer).Begin(nullptr, i);
        (*m_CommandBuffer).PipelineBarrier(&transitionBarriers, nullptr, BarrierDependency::ALL_STAGES);
        (*m_CommandBuffer).End();

        WorkSubmissionDesc workSubmissionDesc = {};
        workSubmissionDesc.physicalDeviceIndex = i;
        workSubmissionDesc.commandBufferNum = 1;
        workSubmissionDesc.commandBuffers = &m_CommandBuffer;

        m_CommandQueue.Submit(workSubmissionDesc, nullptr);
    }

    return m_HelperWaitIdle.WaitIdle();
}