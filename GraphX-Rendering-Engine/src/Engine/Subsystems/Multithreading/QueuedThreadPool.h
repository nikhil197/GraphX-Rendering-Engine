#pragma once

namespace GraphX
{
	/**
	 *	A pool of queued threads for async tasks. This has a predefined number of threads waiting for tasks.
	 *	If all threads are busy, tasks are queued in the pool and are executed when a thread finishes execution of the previous task
	 */
	class QueuedThreadPool
	{
	public:
		QueuedThreadPool()
			: m_Destory(false)
		{}

		/**
		 * Creates the thread pool with the specified number of threads
		 *
		 * @param InNumQueuedThreads Specifies the number of threads to use in the pool
		 * @return Whether the pool creation was successful or not
		 */
		bool Create(uint32_t InNumQueuedThreads);

		/* Cleanup all the background threads */
		void Destroy();

		/* Adds a new job to the pool */
		void AddQueuedWork(class IQueuedWork* InQueuedWork);

		/* Returns the number of threads in the pool */
		uint32_t GetNumThreads() const { return (uint32_t)m_AllThreads.size(); }
		/**
		 * Returns the thread to the waiting threads pool
		 *
		 * @param InThread Thread to be added to the pool
		 * @result Returns the next job, if available
		 */
		class IQueuedWork* GetNextJobOrReturnToPool(class QueuedThread* InThread);

		/* */
		~QueuedThreadPool();

	private:
		/* The work queue */
		std::vector<class IQueuedWork*> m_QueuedJobs;

		/* Threads waiting for work */
		std::vector<class QueuedThread*> m_WaitingThreads;

		/* All the threads in the pool */
		std::vector<class QueuedThread*> m_AllThreads;

		/* Synchronization mutex to sync all the thread pool operations */
		std::mutex m_SyncMutex;

		/* Whether the thread pool is to be destroyed */
		bool m_Destory;
	};

	/* A global thread pool for async tasks */
	extern QueuedThreadPool* g_GlobalThreadPool;
}