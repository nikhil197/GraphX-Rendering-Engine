#pragma once

namespace GraphX
{
	// Reference : Unreal Engine Source Code
	/**
	 * Interface for queued work objects
	 *
	 * This is like IRunnable but doesn't require per thread initialization. 
	 * This is meant to be used in thread pools.
	 */
	class IQueuedWork
	{
	public:
		/**
		 * This is where the actual work is done
		 */
		virtual void DoAsyncWork() = 0;

		/**
		 * Cleanup. This will on be called if the work is not complete
		 */
		virtual void Abandon() = 0;

	public:
		virtual ~IQueuedWork() {}
	};
}