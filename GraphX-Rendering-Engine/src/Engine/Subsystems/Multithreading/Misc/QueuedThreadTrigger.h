#pragma once

namespace GraphX
{
	/* A trigger to signal a queued thread in and out of the idle state */
	class QueuedThreadTrigger
	{
	public:
		/**
		 * Waits for the specified time
		 *
		 * @param WaitTime Time (in milliseconds) for which to wait
		 * @eturn true if the Trigger was signaled, false in case the wait timed out
		 */
		bool Wait(uint32_t WaitTime);

		/* Signal the trigger */
		void Signal();

		/* Reset the trigger signal */
		void Reset();

	private:
		/* Synchronisation primitive to facilitate waking up thread from sleep */
		std::condition_variable m_ConditionVar;

		/* Mutex required by the condition variable */
		std::mutex m_Mutex;

		/* Whether the trigger has been signaled or not */
		bool m_Signaled;
	};
}