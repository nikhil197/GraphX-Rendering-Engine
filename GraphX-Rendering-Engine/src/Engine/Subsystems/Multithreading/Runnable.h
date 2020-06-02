#pragma once

namespace GraphX
{
	/**
	 * Interface for "runnable" objects.
	 *
	 * A runnable object is an object that is "run" on an arbitrary thread. The call usage pattern is
	 * Init(), Run(), Exit(). The thread that is going to "run" this object always uses those calling
	 * semantics. It does this on the thread that is created so that any thread specific uses (TLS, etc.)
	 * are available in the contexts of those calls. A "runnable" does all initialization in Init().
	 *
	 * If initialization fails, the thread stops execution and returns an error code. If it succeeds,
	 * Run() is called where the real threaded work is done. Upon completion, Exit() is called to allow
	 * correct clean up.
	 */
	class IRunnable
	{
	public:

		/**
		 * Initializes the runnable object.
		 *
		 * This method is called in the context of the thread object that aggregates this, not the
		 * thread that passes this runnable to a new thread.
		 *
		 * @return True if initialization was successful, false otherwise
		 * @see Run, Stop, Exit
		 */
		virtual bool Init()
		{
			return true;
		}

		/**
		 * Runs the runnable object.
		 *
		 * This is where all per object thread work is done. This is only called if the initialization was successful.
		 *
		 * @return The exit code of the runnable object
		 * @see Init, Stop, Exit
		 */
		virtual uint32_t Run() = 0;

		/**
		 * Stops the runnable object.
		 *
		 * This is called if a thread is requested to terminate early.
		 * @see Init, Run, Exit
		 */
		virtual void Stop() { }

		/**
		 * Exits the runnable object.
		 *
		 * Called in the context of the aggregating thread to perform any cleanup.
		 * @see Init, Run, Stop
		 */
		virtual void Exit() { }

		/** Virtual destructor */
		virtual ~IRunnable() { }
	};

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