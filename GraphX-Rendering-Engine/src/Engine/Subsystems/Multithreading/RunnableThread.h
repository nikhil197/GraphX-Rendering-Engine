#pragma once

#include "Runnable.h"

namespace GraphX
{
	class RunnableThread
		: public IThread
	{
	public:
		virtual void Suspend(bool ShouldPause) override;

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