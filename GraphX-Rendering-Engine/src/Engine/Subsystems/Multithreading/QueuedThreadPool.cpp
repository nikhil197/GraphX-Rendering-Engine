#include "pch.h"
#include "Subsystems/Multithreading/QueuedThreadPool.h"

#include <mutex>

#include "Subsystems/Multithreading/Base/IQueuedWork.h"
#include "Subsystems/Multithreading/Base/QueuedThread.h"

namespace GraphX
{
	bool QueuedThreadPool::Create(uint32_t InNumQueuedThreads)
	{
		bool Success = true;
		GX_ENGINE_ASSERT(m_AllThreads.size() == 0, "Thread Pool has been already initialised!!");

		std::lock_guard<std::mutex> ScopeLock(m_SyncMutex);

		// Resize the thread arrays
		m_AllThreads.reserve(InNumQueuedThreads);
		m_WaitingThreads.reserve(InNumQueuedThreads);

		// Create Each thread and add it to the array
		for (uint32_t i = 0; i < InNumQueuedThreads && Success; i++)
		{
			QueuedThread* qThread = new QueuedThread;
			if (qThread->Create(this) == true)
			{
				m_AllThreads.emplace_back(qThread);
				m_WaitingThreads.emplace_back(qThread);
			}
			else
			{
				Success = false;
				delete qThread;
			}
		}

		if (Success == false)
		{
			Destroy();
		}

		return Success;
	}

	void QueuedThreadPool::Destroy()
	{
		// If there are threads in the thread pool
		if (m_AllThreads.size() > 0)
		{
			// Complete / Abandon all the tasks
			{
				std::lock_guard<std::mutex> ScopeLock(m_SyncMutex);
				m_Destory = true;
				for (size_t i = 0; i < m_QueuedJobs.size(); i++)
				{
					m_QueuedJobs[i]->Abandon();
				}

				m_QueuedJobs.clear();
			}

			// Wait for all the threads to complete their jobs

			while (true)
			{
				{
					std::lock_guard<std::mutex> ScopeLock(m_SyncMutex);
					if (m_AllThreads.size() == m_WaitingThreads.size())
					{
						break;
					}
				}
			}

			// Delete all the threads
			{
				std::lock_guard<std::mutex> ScopeLock(m_SyncMutex);
				for (size_t i = 0; i < m_AllThreads.size(); i++)
				{
					m_AllThreads[i]->KillThread();
					delete m_AllThreads[i];
				}

				m_AllThreads.clear();
				m_WaitingThreads.clear();
			}
		}
	}

	void QueuedThreadPool::AddQueuedWork(IQueuedWork* InQueuedJob)
	{
		// Check if the pool has been initialised or not
		if (m_AllThreads.size() == 0)
			return;

		GX_ENGINE_ASSERT(InQueuedJob != nullptr, "Passing an invalid job to the thread pool!!")

		// Abandon the task if the pool is being destroyed
		if (m_Destory)
		{
			InQueuedJob->Abandon();
			return;
		}

		QueuedThread* WorkerThread = nullptr;
		{
			std::lock_guard<std::mutex> ScopeLock(m_SyncMutex);

			const size_t AvailableThreadCount = m_WaitingThreads.size();
			if (AvailableThreadCount == 0)
			{
				// No thread is available. Queue the task and return
				m_QueuedJobs.emplace_back(InQueuedJob);
				return;
			}

			WorkerThread = m_WaitingThreads[AvailableThreadCount - 1];

			// Remove the worker thread from the waiting pool
			m_WaitingThreads.pop_back();
		}

		WorkerThread->DoWork(InQueuedJob);
	}

	IQueuedWork* QueuedThreadPool::GetNextJobOrReturnToPool(QueuedThread* InThread)
	{
		GX_ENGINE_ASSERT(InThread != nullptr, "");

		IQueuedWork* NextJob = nullptr;
		
		std::lock_guard<std::mutex> ScopeLock(m_SyncMutex);

		if (m_Destory)
		{
			// If the pool is being destroyed, then there should not be any new job in the waiting queue
			GX_ENGINE_ASSERT(m_QueuedJobs.size() == 0, );
		}

		if (m_QueuedJobs.size() > 0)
		{
			// Get a job from the front of the queue
			NextJob = m_QueuedJobs[0];
			m_QueuedJobs.erase(m_QueuedJobs.begin());
		}

		if (NextJob == nullptr)
		{
			m_WaitingThreads.emplace_back(InThread);
		}

		return NextJob;
	}

	QueuedThreadPool::~QueuedThreadPool()
	{
		Destroy();
	}
}