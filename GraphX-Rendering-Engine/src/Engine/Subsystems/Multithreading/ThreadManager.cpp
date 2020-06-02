#include "pch.h"
#include "ThreadManager.h"

#include "RunnableThread.h"

namespace GraphX
{
	ThreadManager& ThreadManager::Get()
	{
		static ThreadManager s_Manager;
		return s_Manager;
	}

	void ThreadManager::AddThread(const std::thread::id& ThreadID, IThread* InThread)
	{
		std::lock_guard<std::mutex> ScopeLock(m_SyncThreadsMutex);
		if (m_Threads.find(ThreadID) == m_Threads.end())
		{
			m_Threads[ThreadID] = InThread;
		}
	}

	void ThreadManager::RemoveThread(IThread* InThread)
	{
		std::lock_guard<std::mutex> ScopeLock(m_SyncThreadsMutex);
		auto itr = m_Threads.find(InThread->GetID());
		if (itr != m_Threads.end())
		{
			m_Threads.erase(itr);
		}
	}
}