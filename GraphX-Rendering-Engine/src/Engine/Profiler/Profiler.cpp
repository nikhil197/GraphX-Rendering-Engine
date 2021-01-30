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

	const char RunTimeProfiler::s_ProfileFormat[] = ", { \"cat\" : \"Scope\", \"dur\" : %lld, \"name\": \"%s\", \"ph\" : \"X\", \"pid\" : \"0\", \"tid\" : \"0\", \"ts\" : %lld}";
	
	uint32_t RunTimeProfiler::s_FormatStringLen = (uint32_t)strlen(s_ProfileFormat);

	std::vector<char> RunTimeProfiler::s_ProfileString(s_FormatStringLen);

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
		uint32_t requiredBufferSize = s_FormatStringLen + (uint32_t)strlen(Result.Name) + 40; /* 40 for 40 possible characters of the durations in the result */
		if (s_ProfileString.size() < requiredBufferSize)
		{
			s_ProfileString.resize(requiredBufferSize);
		}

		std::snprintf(s_ProfileString.data(), s_ProfileString.size(), s_ProfileFormat, Result.Duration, Result.Name, Result.StartTime);

		if (m_CurrentSession)
		{
			m_ProfilerStream << s_ProfileString.data();
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
	const char MemoryProfiler::s_ProfileFormat[] = ", { \"cat\" : \"Scope\", \"id\" : \"%s\", \"name\": \"%s\", \"ph\" : \"O\", \"pid\" : \"0\", \"tid\" : \"0\", \"ts\" : %lld, \"args\" : {\"snapshot\" : { \"Memory Allocated (in bytes)\" : %u,  \"Memory Freed (in bytes)\" : %u}}}";

	uint32_t MemoryProfiler::s_FormatStringLen = (uint32_t)strlen(s_ProfileFormat);

	std::vector<char> MemoryProfiler::s_ProfileString(s_FormatStringLen);

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
		uint32_t requiredBufferSize = s_FormatStringLen + (uint32_t)strlen(Result.Name) * 2 + 40; /* 40 for 40 possible characters of the durations and sizes in the result */
		if (s_ProfileString.size() < requiredBufferSize)
		{
			s_ProfileString.resize(requiredBufferSize);
		}

		std::snprintf(s_ProfileString.data(), s_ProfileString.size(), s_ProfileFormat, Result.Name, Result.Name, Result.Time, Result.MemAllocated, Result.MemFreed);

		if (m_CurrentSession)
		{
			m_ProfilerStream << s_ProfileString.data();
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