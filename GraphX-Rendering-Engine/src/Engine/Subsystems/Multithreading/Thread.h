#pragma once

namespace GraphX
{
	class IRunnable;

	/**
	* Interface for threads
	*/
	class IThread
	{
	public:
		/**
		 * Factory method to create a thread.
		 *
		 * @param InRunnable The runnable object to execute
		 * @param ThreadName Name of the thread
		 * @return The newly created thread or nullptr if it failed
		 */
		static IThread* Create(IRunnable* InRunnable, const std::string& ThreadName);

		/**
		 * Tells the thread to either pause execution or resume depending on the
		 * passed in value.
		 *
		 * @param bShouldPause Whether to pause the thread (true) or resume (false)
		 */
		virtual void Suspend(bool ShouldPause = true) = 0;

		/**
		 * Tells the thread to exit. If the caller needs to know when the thread has exited, it should use the bShouldWait value.
		 * It's highly recommended not to kill the thread without waiting for it.
		 * Having a thread forcibly destroyed can cause leaks and deadlocks.
		 *
		 * The kill method is calling Stop() on the runnable to kill the thread gracefully.
		 *
		 * @param bShouldWait	If true, the call will wait infinitely for the thread to exit.
		 * @return Always true
		 */
		virtual bool Kill(bool ShouldWait = true) = 0;

		/** Halts the caller until this thread is has completed its work. */
		virtual void WaitForCompletion() = 0;

		/* Returns the name of the thread */
		const std::string& GetThreadName() const { return m_ThreadName; }

		/* Returns the thread ID */
		const std::thread::id& GetID() const { return m_ThreadID; }

		/** Default constructor. */
		IThread();

		/** Virtual destructor */
		virtual ~IThread();

	protected:
		virtual bool CreateInternal(IRunnable* InRunnable, const std::string& ThreadName) = 0;

	protected:
		/** Holds the name of the thread. */
		std::string m_ThreadName;

		/** The runnable object to execute on this thread. */
		IRunnable* m_Runnable;

		/** ID set during thread creation. */
		std::thread::id m_ThreadID;
	};
}