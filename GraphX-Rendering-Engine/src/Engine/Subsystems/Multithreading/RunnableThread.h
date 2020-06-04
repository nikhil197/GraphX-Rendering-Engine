#pragma once

#include "Thread.h"

namespace GraphX
{
	class RunnableThread
		: public IThread
	{
	public:
		RunnableThread();

		~RunnableThread();

		// IThread Interface
		virtual void Suspend(bool ShouldPause = true) override;

		virtual bool Kill(bool ShouldWait = true) override;

		virtual void WaitForCompletion() override;

	private:
		virtual bool CreateInternal(IRunnable* InRunnable, const std::string& ThreadName) override;

		/**
		* The real thread entry point. It calls the Init/Run/Exit methods on
		* the runnable object
		*/
		uint32_t Run();

	private:
		/** Holds the thread object. */
		std::thread* m_Thread;
	};
}