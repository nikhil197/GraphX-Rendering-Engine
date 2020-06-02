#include "pch.h"
#include <thread>

#include "Runnable.h"
#include "RunnableThread.h"
#include "ThreadManager.h"

namespace GraphX
{
	IThread::IThread()
		: m_ThreadName(""), m_Runnable(nullptr), m_ThreadID(0)
	{}

	IThread::~IThread()
	{
		if (m_Runnable)
		{
			ThreadManager::Get().RemoveThread(this);
		}
	}

	IThread* IThread::Create(IRunnable* InRunnable, const std::string& ThreadName)
	{
		IThread* Thread = new RunnableThread();

		if (Thread->CreateInternal(InRunnable, ThreadName) == false)
		{
			delete Thread;
		}
	}

	/***************** Runnable Thread ********************/
	bool RunnableThread::CreateInternal(IRunnable* InRunnable, const std::string& ThreadName)
	{
		m_Runnable = InRunnable;
		m_ThreadName = ThreadName;

		m_Thread = new std::thread(&RunnableThread::Run, this);
		m_ThreadID = m_Thread->get_id();

		ThreadManager::Get().AddThread(m_ThreadID, this);

		return m_Thread != nullptr;
	}

	void RunnableThread::Suspend(bool ShouldPause)
	{
		// Using the WinAPI directly here to pause / resume threads
		if (ShouldPause)
		{
			SuspendThread(m_Thread->native_handle());
		}
		else
		{
			ResumeThread(m_Thread->native_handle());
		}
	}

	uint32_t RunnableThread::Run()
	{
		// Assume the Initialisation will fail
		uint32_t ExitCode = 1;
		GX_ENGINE_ASSERT(m_Runnable != nullptr,);

		if (m_Runnable->Init() == true)
		{
			// Rrun the task that needs to be done
			ExitCode = m_Runnable->Run();
			// Allow any allocated resources to be cleaned up
			m_Runnable->Exit();
		}

		return ExitCode;
	}
}