#pragma once

#include "Subsystems/Multithreading/Base/IRunnable.h"

namespace GraphX
{
	// Reference: Unreal Engine Source Code
	/**
	 * Poolable threads. Life Cycle of Poolable threads is different from a normal thread.
	 * Queued Threads spend most of their life cycle idle, waiting for next task. When signaled,
	 * they perform a job and then return themselves to their owning pool and return to idle state
	 */
	class QueuedThread
		: public IRunnable
	{
	public:
		QueuedThread()
			: m_QueuedWork(nullptr), m_OwnerPool(nullptr), m_Thread(nullptr), m_Trigger(nullptr), m_Exit(false)
		{}

		/**
		 * Creates the thread with the specified stack size and creates the various
		 * events to be able to communicate with it.
		 *
		 * @param InThreadPool The thread pool interface used to place this thread back into the pool of available threads when its work is done
		 * @return True if the thread and all of its initialization was successful, false otherwise
		 */
		bool Create(class QueuedThreadPool* InThreadPool);

		// IRunnable Interface
		virtual uint32_t Run() override;

		// IRunnable Interface ---------- END

		/**
		 * Tells the thread to exit. Must be called before deleting the thread
		 *
		 * @return True if the thread exited graceful, false otherwise
		 */
		bool KillThread();

		/**
		 * Tells the thread there is work to be done. Upon completion, the thread
		 * is responsible for adding itself back into the available pool.
		 *
		 * @param InQueuedWork The queued work to perform
		 */
		void DoWork(class IQueuedWork* InQueuedWork);

		/* Destructor */
		~QueuedThread() = default;

	private:
		/* The work this thread is doing */
		class IQueuedWork* m_QueuedWork;

		/* Owning thread pool */
		class QueuedThreadPool* m_OwnerPool;

		/* Actual thread for the queued thread */
		class IThread* m_Thread;

		/* Trigger used to signal the thread out of idle state */
		class QueuedThreadTrigger* m_Trigger;

		/* Whether the thread should exit (Needs to be atomic, since thread will be killed by another thread (call to KillThread() method) */
		std::atomic<bool> m_Exit;
	};
}