#pragma once

#include "BaseDLL.h"

namespace nri
{
	class NRI_BASE_API CommandQueue {
	public:
		virtual ~CommandQueue() {}

		virtual void SetDebugName(const char* name) = 0;

		virtual void Submit(const WorkSubmissionDesc& workSubmissionDesc, DeviceSemaphore* deviceSemaphore) = 0;
		virtual void Wait(DeviceSemaphore& deviceSemaphore) = 0;


		virtual   Result ChangeResourceStates(const TransitionBarrierDesc& transitionBarriers) = 0;
		virtual  Result UploadData(const TextureUploadDesc* textureUploadDescs, uint32_t textureUploadDescNum,
			const BufferUploadDesc* bufferUploadDescs, uint32_t bufferUploadDescNum) = 0;
		virtual  Result WaitForIdle() = 0;
	};
}