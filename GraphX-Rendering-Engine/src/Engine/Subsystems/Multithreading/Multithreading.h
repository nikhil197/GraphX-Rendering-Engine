#pragma once

// Only Include file required for all multi-threading related tasks

#include "Async/AsyncTask.h"
#include "QueuedThreadPool.h"

namespace GraphX
{
	/**
	 * For initializing and shutting down multi-threading subsystem. Also provides (TODO: will provide) some helper functions.
	 */
	class Multithreading
	{
	public:
		/**
		 * Initialize all the multi threading stuff that needs to be initialized before any other work is done
		 */
		static void Init();

		/**
		 * Cleans up all the resources used by multi threading constructs
		 */
		static void Shutdown();

	};
}