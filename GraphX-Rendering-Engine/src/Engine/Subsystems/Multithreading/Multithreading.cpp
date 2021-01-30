#include "pch.h"
#include "Multithreading.h"

#include "Subsystems/Multithreading/QueuedThreadPool.h"

namespace GraphX
{
	/* A global thread pool for executing tasks asynchronously */
	QueuedThreadPool* g_GlobalThreadPool = nullptr;

	void Multithreading::Init()
	{
		// Create the global thread pool
		GX_ENGINE_ASSERT(g_GlobalThreadPool == nullptr, "Global Thread Pool already initialised!!");
		g_GlobalThreadPool = new class QueuedThreadPool;
		bool res = g_GlobalThreadPool->Create(EngineConstants::GThreadPoolThreadCount);
		if (res == false)
		{
			delete g_GlobalThreadPool;
			g_GlobalThreadPool = nullptr;
			GX_ENGINE_ASSERT(false, "Error while creating the global thread pool!!");
		}
	}

	void Multithreading::Shutdown()
	{
		// Delete the global thread pool
		GX_ENGINE_ASSERT(g_GlobalThreadPool != nullptr, "Global thread pool already deleted!!");
		delete g_GlobalThreadPool;
		g_GlobalThreadPool = nullptr;
	}
}