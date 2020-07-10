#pragma once

#include <future>
#include <functional>

#include "Subsystems/Multithreading/Runnable.h"
#include "Subsystems/Multithreading/RunnableThread.h"
#include "Subsystems/Multithreading/QueuedWork.h"
#include "Subsystems/Multithreading/QueuedThreadPool.h"

namespace GraphX
{
	// NOTE: Referenced from Unreal Engine Source Code
	enum class AsyncExecutionPolicy
	{
		/* Execution on a separate thread */
		Thread,

		/* Execution in a queued thread pool */
		ThreadPool
	};

	template<typename Result>
	inline void SetPromiseValue(std::promise<Result>& promise, const std::function<Result()>& Function)
	{
		promise.set_value(Function());
	}

	template<>
	inline void SetPromiseValue(std::promise<void>& promise, const std::function<void()>& Function)
	{
		Function();
		promise.set_value();
	}

	/*
	* Template for asynchronous functions that can be executed on a separate thread
	*/
	template<typename Type>
	class AsyncRunnableTask
		: public IRunnable
	{
	public:
		AsyncRunnableTask(std::function<Type()>&& InFunction, std::promise<Type>&& InPromise, std::future<IThread*>&& InFuture)
			: m_Function(std::move(InFunction)), m_Promise(std::move(InPromise)), m_Future(std::move(InFuture))
		{}

		// IRunnable Interface
		virtual uint32_t Run() override
		{
			SetPromiseValue(m_Promise, m_Function);
			
			return 0;
		}

		virtual void Exit() override
		{
			// Find a better way to do this
			// 1st Alt -> (Delete this task and the thread executing this task on a separate thread)
			IThread* Thread = m_Future.get();
			Thread->m_AutoDelete = true;

			// Auto Delete the task
			delete this;
		}

		// IRunnable Interface -------- END
	private:
		/* Function to be executed asynchronously */
		std::function<Type()> m_Function;

		/* Object which is responsible for setting the result */
		std::promise<Type> m_Promise;

		/* Result of the executed function */
		std::future<IThread*> m_Future;
	};

	/*
	* Template for asynchronous functions that can be executed in a thread pool
	*/
	template<typename Type>
	class AsyncQueuedTask
		: public IQueuedWork
	{
	public:
		AsyncQueuedTask(std::function<Type()>&& InFunction, std::promise<Type>&& InPromise)
			: m_Function(std::move(InFunction)), m_Promise(std::move(InPromise))
		{}

	public:
		// IQueuedTask Interface
		virtual void DoAsyncWork() override
		{
			SetPromiseValue(m_Promise, m_Function);
			delete this;
		}

		virtual void Abandon() override
		{

		}

		// IQueuedTask Interface -------- END

	private:
		/* Function to be executed asynchronously */
		std::function<Type()> m_Function;

		/* Object which is responsible for setting the result */
		std::promise<Type> m_Promise;
	};

	/**
	* Execute a given function asynchronously.
	*
	* @param ResultType The type of the function's return value.
	* @param Execution The execution method to use, i.e. on a separate thread or in a thread pool.
	* @param Function The function to execute.
	* @return A TFuture object that will receive the return value from the function.
	*/
	template<typename Result>
	std::future<Result> Async(AsyncExecutionPolicy ExecutionPolicy, std::function<Result()> InFunction)
	{
		std::promise<Result> Promise;
		std::future<Result> Future = Promise.get_future();

		switch (ExecutionPolicy)
		{
			case AsyncExecutionPolicy::Thread:
				{
					std::promise<IThread*> TPromise;
					AsyncRunnableTask<Result>* Runnable = new AsyncRunnableTask<Result>(std::move(InFunction), std::move(Promise), std::move(TPromise.get_future()));
					IThread* Thread = IThread::Create(Runnable, "Async ");	// TODO: Add a Thread safe counter here for the created threads

					TPromise.set_value(Thread);
				}
				break;

			case AsyncExecutionPolicy::ThreadPool:
				{
					//TODO
					g_GlobalThreadPool->AddQueuedWork(new AsyncQueuedTask<Result>(std::move(InFunction), std::move(Promise)));
				}
				break;
		}

		return Future;
	}
}