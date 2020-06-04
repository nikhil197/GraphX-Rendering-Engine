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
}