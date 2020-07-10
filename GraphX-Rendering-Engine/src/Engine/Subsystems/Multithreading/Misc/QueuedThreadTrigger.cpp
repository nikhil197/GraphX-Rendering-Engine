#include "pch.h"

#include <condition_variable>
#include <mutex>

#include "QueuedThreadTrigger.h"

namespace GraphX
{
	bool QueuedThreadTrigger::Wait(uint32_t WaitTime)
	{
		std::unique_lock<std::mutex> ScopeLock(m_Mutex);
		return m_ConditionVar.wait_for(ScopeLock, std::chrono::milliseconds(WaitTime), [&] { return m_Signaled; });
	}

	void QueuedThreadTrigger::Signal()
	{
		{
			std::lock_guard<std::mutex> ScopeLock(m_Mutex);
			m_Signaled = true;
		}
		
		// Notify the thread waiting on this variable
		m_ConditionVar.notify_one();
	}

	void QueuedThreadTrigger::Reset()
	{
		std::lock_guard<std::mutex> ScopeLock(m_Mutex);
		m_Signaled = false;
	}
}