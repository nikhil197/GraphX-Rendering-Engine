#include "pch.h"

#include <thread>
#include <atomic>

#include "Subsystems/Multithreading/Base/IRunnable.h"
#include "Subsystems/Multithreading/Base/RunnableThread.h"
#include "Subsystems/Multithreading/Base/QueuedThread.h"

#include "Subsystems/Multithreading/ThreadManager.h"

#include "Subsystems/Multithreading/QueuedThreadPool.h"
#include "Subsystems/Multithreading/Async/AsyncTask.h"

#include "Subsystems/Multithreading/Base/IQueuedWork.h"
#include "Subsystems/Multithreading/Misc/QueuedThreadTrigger.h"

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
		// TODO: First check if multithreading is supported

		IThread* Thread = new RunnableThread();

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
			//m_Thread->join();

			delete m_Thread;
			m_Thread = nullptr;
		}
	}

	void RunnableThread::Suspend(bool ShouldPause)
	{
		GX_ENGINE_ASSERT(m_Thread != nullptr, "Thread not created properly!!");
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
			// Run the task that needs to be done
			ExitCode = m_Runnable->Run();
			// Allow any allocated resources to be cleaned up
			m_Runnable->Exit();
		}

		return ExitCode;
	}

	/***************** Queued Thread ********************/
	bool QueuedThread::Create(QueuedThreadPool* InThreadPool)
	{
		static uint32_t PoolThreadIndex = 0;
		static std::string PoolThreadFormat("Pool Thread %d");

		std::string ThreadName;
		std::snprintf(&ThreadName[0], PoolThreadFormat.length() + 2, PoolThreadFormat.c_str(), PoolThreadIndex);
		PoolThreadIndex++;

		m_OwnerPool = InThreadPool;
		m_Trigger = new QueuedThreadTrigger;
		m_Thread = IThread::Create(this, ThreadName);

		return true;
	}

	uint32_t QueuedThread::Run()
	{
		while (!m_Exit.load(std::memory_order_relaxed))
		{
			// Wait for the next job
			bool WaitForTask = true;
			while (WaitForTask)
			{
				WaitForTask = !m_Trigger->Wait(EngineConstants::QueuedThreadWaitTime);
			}

			IQueuedWork* CurrentJob = m_QueuedWork;
			m_QueuedWork = nullptr;

			// DO the current job
			while (CurrentJob)
			{
				// Do the job
				CurrentJob->DoAsyncWork();

				// Fetch the next job
				CurrentJob = m_OwnerPool->GetNextJobOrReturnToPool(this);
			}

			// Set the trigger state to not signaled
			m_Trigger->Reset();
		}

		return 0;
	}

	bool QueuedThread::KillThread()
	{
		GX_ENGINE_ASSERT(m_Exit == false, "Thread Has already been deleted");

		bool ExitedOk = true;
		m_Exit = true;

		// Trigger the thread out of waiting state
		m_Trigger->Signal();

		// Wait for the thread to finish its job
		m_Thread->WaitForCompletion();

		delete m_Trigger;
		delete m_Thread;

		return ExitedOk;
	}

	void QueuedThread::DoWork(IQueuedWork* InQueuedWork)
	{
		GX_ENGINE_ASSERT(m_QueuedWork == nullptr, "Thread is already doing another job!!");

		m_QueuedWork = InQueuedWork;

		// Trigger the thread out of waiting state
		m_Trigger->Signal();
	}
}