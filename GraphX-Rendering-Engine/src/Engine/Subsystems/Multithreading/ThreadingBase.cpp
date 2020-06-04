#include "pch.h"
#include <thread>

#include "Runnable.h"
#include "RunnableThread.h"
#include "ThreadManager.h"

namespace GraphX
{
	IThread::IThread()
		: m_ThreadName(""), m_Runnable(nullptr), m_ThreadID()
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
		// TODO: First check if multithread is supported
		
		IThread* Thread = new RunnableThread();;

		if (Thread->CreateInternal(InRunnable, ThreadName) == false)
		{
			delete Thread;
			Thread = nullptr;
		}

		return Thread;
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

	RunnableThread::RunnableThread()
		: m_Thread(nullptr)
	{}

	RunnableThread::~RunnableThread()
	{
		if (m_Thread)
		{
			delete m_Thread;
			m_Thread = nullptr;
		}
	}

	void RunnableThread::Suspend(bool ShouldPause)
	{
		// Using the WinAPI directly here to pause / resume threads (Might need to change this, as the standard might not work properly with platform specific API)
		// (OR use only the Win32API in place of c++ standard for the windows platform)
		if (ShouldPause)
		{
			::SuspendThread(m_Thread->native_handle());
		}
		else
		{
			::ResumeThread(m_Thread->native_handle());
		}
	}

	bool RunnableThread::Kill(bool ShouldWait)
	{
		GX_ENGINE_ASSERT(m_Thread != nullptr, "Thread not created properly!!");
		bool ExitedOk = true;

		if (m_Runnable)
		{
			m_Runnable->Stop();
		}

		if (ShouldWait == true)
		{
			// Wait for the thread to finish as it may have shared locks and stuff which can cause other threads to dead-lock
			m_Thread->join();
		}

		delete m_Thread;
		m_Thread = nullptr;

		return ExitedOk;
	}

	void RunnableThread::WaitForCompletion()
	{
		// Block until thread finished its execution
		m_Thread->join();
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