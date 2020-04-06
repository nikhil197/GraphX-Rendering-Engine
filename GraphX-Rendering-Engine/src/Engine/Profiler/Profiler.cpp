#include "pch.h"

#include "Profiler.h"

#if GX_PROFILING && GX_PROFILE_MEMORY
	void* operator new (size_t count)
	{

		::GraphX::MemoryProfiler::Get().MemCounters.MemoryAllocated += count;
		return malloc(count);
	}

	void operator delete(void* ptr, size_t count)
	{
		::GraphX::MemoryProfiler::Get().MemCounters.MemoryFreed += count;
		free(ptr);
	}
#endif

namespace GraphX
{
	/******************* RunTime Profiler *********************/
	void RunTimeProfiler::BeginSession(const char* name, const std::string& filePath)
	{
		if (m_CurrentSession)
		{
			if (Log::GetEngineLogger())
			{
				GX_ENGINE_ERROR("RunTimeProfiler::BeginSession({0}) while '{1}' is already open", name, m_CurrentSession->Name);
			}

			EndSession_Internal();
		}
		else
		{
			size_t LastPos = filePath.find_last_of(".");
			std::string FileName = filePath.substr(0, LastPos);

			m_ProfilerStream.open(FileName + "_RunTime.json");

			if (m_ProfilerStream.is_open())
			{
				m_CurrentSession = new ProfilerSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetEngineLogger())
				{
					GX_ENGINE_ERROR("Profiler could not open the {0} file.", filePath);
				}
			}
		}
	}

	void RunTimeProfiler::WriteProfile(const RunTimeProfilerResult&& Result)
	{
		std::stringstream profile;

		profile << ",{\"cat\" : \"Scope\", ";
		profile << "\"dur\" : " << Result.Duration << ',';
		profile << "\"name\" : \"" << Result.Name << "\", ";
		profile << "\"ph\" : \"X\", ";
		profile << "\"pid\" : \"0\", ";
		profile << "\"tid\" : \"0\", ";
		profile << "\"ts\" : " << Result.StartTime;
		profile << "}";

		if (m_CurrentSession)
		{
			m_ProfilerStream << profile.str();
			m_ProfilerStream.flush();
		}
	}

	void RunTimeProfiler::EndSession()
	{
		EndSession_Internal();
	}

	void RunTimeProfiler::WriteHeader()
	{
		m_ProfilerStream << "{ \" otherData\" : { \" version\" : \" GraphX Engine v1.0 \"}, \"traceEvents\" : [{}";
		m_ProfilerStream.flush();
	}

	void RunTimeProfiler::WriteFooter()
	{
		m_ProfilerStream << "]}";
		m_ProfilerStream.flush();
	}

	void RunTimeProfiler::EndSession_Internal()
	{
		if (m_CurrentSession)
		{
			WriteFooter();
			m_ProfilerStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
		}
	}

	/******************* Memory Profiler *********************/
	void MemoryProfiler::BeginSession(const char* name, const std::string& filePath)
	{
		if (m_CurrentSession)
		{
			if (Log::GetEngineLogger())
			{
				GX_ENGINE_ERROR("MemoryProfiler::BeginSession({0}) while '{1}' is already open", name, m_CurrentSession->Name);
			}

			EndSession_Internal();
		}
		else
		{
			size_t LastPos = filePath.find_last_of('.');
			std::string FileName = filePath.substr(0, LastPos);

			m_ProfilerStream.open(FileName + "_MemUse.json");

			if (m_ProfilerStream.is_open())
			{
				m_CurrentSession = new ProfilerSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetEngineLogger())
				{
					GX_ENGINE_ERROR("Profiler could not open the {0} file.", filePath);
				}
			}
		}
	}

	void MemoryProfiler::WriteProfile(MemoryProfilerResut&& Result)
	{
		std::stringstream profile;

		profile << ",{\"cat\" : \"Scope\", ";
		profile << "\"id\" : \"" << Result.Name << "\",";
		profile << "\"name\" : \"" << Result.Name << "\", ";
		profile << "\"ph\" : \"O\", ";
		profile << "\"pid\" : \"0\", ";
		profile << "\"tid\" : \"0\", ";
		profile << "\"ts\" : " << Result.Time << ", ";
		profile << "\"args\" : {\"snapshot\": { \"Memory Allocated (in bytes)\" : " << Result.MemAllocated;
		profile << ", \"Memory Freed (in bytes)\" : " << Result.MemFreed;
		profile << "}}}";

		if (m_CurrentSession)
		{
			m_ProfilerStream << profile.str();
			m_ProfilerStream.flush();
		}
	}

	void MemoryProfiler::EndSession()
	{
		EndSession_Internal();
	}

	/* NOTE: DO NOT USE THIS METHOD. ONLY FOR INTERNAL USE */
	void MemoryProfiler::ResetMemCounters()
	{
		MemCounters.MemoryAllocated = 0;
		MemCounters.MemoryFreed = 0;
	}
	
	void MemoryProfiler::WriteHeader()
	{
		m_ProfilerStream << "{ \" otherData\" : { \" version\" : \" GraphX Engine v1.0 \"}, \"traceEvents\" : [{}";
		m_ProfilerStream.flush();
	}

	void MemoryProfiler::WriteFooter()
	{
		m_ProfilerStream << "]}";
		m_ProfilerStream.flush();
	}

	void MemoryProfiler::EndSession_Internal()
	{
		if (m_CurrentSession)
		{
			WriteFooter();
			m_ProfilerStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
		}
	}
}