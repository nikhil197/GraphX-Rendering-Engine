#pragma once

#include <chrono>
#include <string>
#include <ostream>

#include "Engine/Log.h"
#include "Engine/EngineConfig.h"
#include "Engine/Utilities/EngineUtil.h"

namespace GraphX
{
	#if GX_PROFILING 
		#if GX_PROFILE_MEMORY
			#define START_MEMORY_COUNTERS_DECL \
					uint32_t m_StartMemAllocated;\
					uint32_t m_StartMemFreed;

			#define INIT_MEM_COUNTERS() \
				{\
					const MemoryCounters& counters = ::GraphX::MemoryProfiler::Get().MemCounters;\
					m_StartMemAllocated = counters.MemoryAllocated;\
					m_StartMemFreed = counters.MemoryFreed;\
				}

			#define CALCULATE_MEM_COUNTERS() \
				{\
					const MemoryCounters& counters = ::GraphX::MemoryProfiler::Get().MemCounters;\
					uint32_t MemAllocated = counters.MemoryAllocated - m_StartMemAllocated;\
					uint32_t MemFreed = counters.MemoryFreed - m_StartMemFreed;\
					MemoryProfiler::Get().WriteProfile({ m_Name, MemAllocated, MemFreed, startTime + duration });\
				}
		#else
			#define START_MEMORY_COUNTERS_DECL
			#define INIT_MEM_COUNTERS()
			#define CALCULATE_MEM_COUNTERS()
		#endif
	#else
		#define START_MEMORY_COUNTERS_DECL
		#define INIT_MEM_COUNTERS()
		#define CALCULATE_MEM_COUNTERS()
	#endif

	struct RunTimeProfilerResult
	{
		const char* Name;

		long long StartTime;
		long long Duration;
	};

	struct MemoryProfilerResut
	{
		const char* Name;

		uint32_t MemAllocated;
		uint32_t MemFreed;
		long long Time;
	};

	struct ProfilerSession
	{
		const char* Name;
	};

	struct MemoryCounters
	{
		uint32_t MemoryAllocated = 0;
		uint32_t MemoryFreed = 0;
	};

	class RunTimeProfiler
	{
	private:
		ProfilerSession* m_CurrentSession;
		std::ofstream m_ProfilerStream;

	public:
		RunTimeProfiler()
			: m_CurrentSession(nullptr)
		{ }

		void BeginSession(const char* name, const std::string& filePath = "Profiler-Results.json");

		void WriteProfile(const RunTimeProfilerResult&& Result);

		void EndSession(); 

		static RunTimeProfiler& Get()
		{
			static RunTimeProfiler s_Profiler;
			return s_Profiler;
		}

	private:
		void WriteHeader();

		void WriteFooter();

		void EndSession_Internal();
	};

	class MemoryProfiler
	{
	private:
		ProfilerSession* m_CurrentSession;
		std::ofstream m_ProfilerStream;

	public:
		MemoryCounters MemCounters;

	public:
		MemoryProfiler()
			: m_CurrentSession(nullptr)
		{ }

		void BeginSession(const char* name, const std::string& filePath = "Profiler-Results.json");
		
		void WriteProfile(MemoryProfilerResut&& Result);

		void EndSession();

		/* NOTE: DO NOT USE THIS METHOD. ONLY FOR INTERNAL USE */
		void ResetMemCounters();

		static MemoryProfiler& Get()
		{
			static MemoryProfiler s_Profiler;
			return s_Profiler;
		}

	private:
		void WriteHeader();

		void WriteFooter();

		void EndSession_Internal();
	};

	class ProfilerInstrument
	{
		using HighResDuration = std::chrono::duration<long long int, std::micro>;
	public:
		ProfilerInstrument(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimePoint = std::chrono::high_resolution_clock::now();
			INIT_MEM_COUNTERS()
		}

		~ProfilerInstrument()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			std::chrono::high_resolution_clock::time_point endPoint = std::chrono::high_resolution_clock::now();
			long long startTime = std::chrono::time_point_cast<HighResDuration>(m_StartTimePoint).time_since_epoch().count();
			long long duration = std::chrono::time_point_cast<HighResDuration>(endPoint).time_since_epoch().count() - startTime;

			RunTimeProfiler::Get().WriteProfile({ m_Name, startTime, duration });

			m_Stopped = true;

			CALCULATE_MEM_COUNTERS()
		}

	private:
		const char* m_Name;
		std::chrono::high_resolution_clock::time_point m_StartTimePoint;
		bool m_Stopped;
		
		START_MEMORY_COUNTERS_DECL
	};
}


#if GX_PROFILING
	#if GX_PROFILE_MEMORY
		#define GX_PROFILER_BEGIN_SESSION(name, filePath)\
			::GraphX::RunTimeProfiler::Get().BeginSession(name, filePath);\
			::GraphX::MemoryProfiler::Get().BeginSession(name, filePath);
		
		#define GX_PROFILER_END_SESISON()\
			::GraphX::RunTimeProfiler::Get().EndSession();\
			::GraphX::MemoryProfiler::Get().EndSession();

		#define GX_PROFILER_NEW_FRAME()						::GraphX::MemoryProfiler::Get().ResetMemCounters();
	#else
		#define GX_PROFILER_BEGIN_SESSION(name, filePath)	::GraphX::RunTimeProfiler::Get().BeginSession(name, filePath);
		#define GX_PROFILER_END_SESISON()					::GraphX::RunTimeProfiler::Get().EndSession();
		#define GX_PROFILER_NEW_FRAME()
	#endif
	#define GX_PROFILE_SCOPE(name)						::GraphX::ProfilerInstrument instrument##__LINE__(name);
	#define GX_PROFILE_FUNCTION()						GX_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define GX_PROFILER_BEGIN_SESSION(name, filePath)	
	#define GX_PROFILER_END_SESISON()
	#define GX_PROFILER_NEW_FRAME()
	#define GX_PROFILE_SCOPE(name)
	#define GX_PROFILE_FUNCTION() 
#endif