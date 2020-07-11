#pragma once

#include <atomic>

#include "Subsystems/Multithreading/QueuedWork.h"
#include "Subsystems/Multithreading/QueuedThreadPool.h"

namespace GraphX
{
	// Reference: Unreal Engine Source Code
	/**
	 * Template for jobs executed in a thread pool (Task and job is used interchangeably)
	 *
	 * NOTE: 'Task' must have two methods -> DoWork() && bool CanAbandon()
	 *
	 *	Usage Example:
	 * class ExampleTask
	 *	{
	 *		void DoWork()
	 *		{
	 *			....
	 *		}
	 *
	 *		bool CanAbandon() {return false;}
	 *	};
	 *
	 * AsyncQueueTask<ExampleTask> job = new AsyncQueueTask<ExampleTask>(...Args...);
	 * job.StartBackgroundTask(g_GlobalThreadPool);
	 */
	template<typename Task>
	class AsyncQueuedWork
		: public IQueuedWork
	{
	public:
		template<typename... Args>
		AsyncQueuedWork(Args&& ...args)
			: m_WorkFinished(false), Task(std::forward<Args>(args)), m_ThreadPool(nullptr)
		{}

		// IQueuedWork Interface

		// Only called from a different thread in a thread pool
		virtual void DoAsyncWork() override
		{
			DoWork();
		}

		// Only called from a different thread in a thread pool
		virtual void Abandon() override
		{
			// If task can be abandoned, abandon it
			if (m_Task.CanAbandon())
			{
				m_Task.Abandon();
			}
			else
			{
				// Complete the task now
				DoWork();
			}
		}

		// IQueuedWork Interface -------- END

		// Run the task now, on this thread
		void StartSynchronousTask()
		{
			Start(true, nullptr);
		}

		// TODO: Add functionality to wait for completion

		/**
		* Run the task on a thread pool, in background
		*
		* @param InThreadPool ThreadPool on which to run this task (Default - GlobalThreadPool)
		*/
		void StartBackgroundTask(QueuedThreadPool* InThreadPool)
		{
			Start(false, InThreadPool);
		}

		/* Returns the embedded user job */
		Task& GetTask() const
		{
			return m_Task;
		}

		// Returns if the task is finished
		bool IsFinished() const
		{
			return m_WorkFinished.load() == true;
		}

	private:

		void Start(bool InForceSynchronous, QueuedThreadPool* InThreadPool)
		{
			m_ThreadPool = InThreadPool;
			m_WorkFinished = false;

			if (InForceSynchronous)
			{
				m_ThreadPool = nullptr;
			}

			if (m_ThreadPool != nullptr)
			{
				m_ThreadPool->AddQueuedWork(this);
			}
			else
			{
				DoWork();
			}
		}

		void DoWork()
		{
			m_Task.DoWork();

			// Set the work finished to true
			m_WorkFinished = true;
		}

	private:
		/* If the work is finished */
		std::atomic<bool> m_WorkFinished;

		/* User job embedded in this */
		Task m_Task;

		/* Thread pool currently executing this job */
		class QueuedThreadPool* m_ThreadPool;
	};
}