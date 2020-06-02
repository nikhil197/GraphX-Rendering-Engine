#pragma once

namespace GraphX
{
	class IThread;

	/**
	 * Manages runnables and runnable threads.
	 */
	class ThreadManager
	{
	public:
		/**
		* Used internally to add a new thread object.
		*
		* @param Thread thread object.
		* @see RemoveThread
		*/
		void AddThread(const std::thread::id& ThreadId, IThread* Thread);

		/**
		* Used internally to remove thread object.
		*
		* @param Thread thread object to be removed.
		* @see AddThread
		*/
		void RemoveThread(IThread* Thread);

		/* Returns the singleton thread manager object */
		static ThreadManager& Get();

	private:
		/* Mutex to synchronise the Thread manager access */
		std::mutex m_SyncThreadsMutex;

		/* Maps all the threads */
		std::unordered_map<std::thread::id, IThread*> m_Threads;
	};
}