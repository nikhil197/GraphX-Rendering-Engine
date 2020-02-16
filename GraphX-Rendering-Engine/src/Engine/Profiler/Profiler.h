#pragma once

#include <chrono>
#include <string>
#include <ostream>

#include "Engine/Log.h"
#include "Engine/Utilities/EngineUtil.h"

namespace GraphX
{
	struct ProfilerResult
	{
		std::string Name;

		double StartTime;
		double Duration;
	};

	struct ProfilerSession
	{
		const char* Name;
	};

	class Profiler
	{
	private:
		ProfilerSession* m_CurrentSession;
		std::ofstream m_ProfilerStream;

	public:
		Profiler()
			: m_CurrentSession(nullptr)
		{ }

		void BeginSession(const char* name, const std::string& filePath = "Profiler-Results.json")
		{
			if (m_CurrentSession)
			{
				if (Log::GetEngineLogger())
				{
					GX_ENGINE_ERROR("CpuProfiler::BeginSession({0}) while '{1}' is already open", name, m_CurrentSession->Name);
				}

				EndSession_Internal();
			}
			else
			{
				m_ProfilerStream.open(filePath);

				if (m_ProfilerStream.is_open())
				{
					m_CurrentSession = new ProfilerSession({name});
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

		void WriteProfile(const ProfilerResult& Result)
		{
			std::stringstream profile;

			profile << "{\"cat\" : \"Scope\", ";
			profile << "\"dur\" : " << Result.Duration << ',';
			profile << "\"name\" : \"" << Result.Name << "\", ";
			profile << "\"ph\" : \"X\", ";
			profile << "\"pid\" : \"0\", ";
			profile << "\"tid\" : \"0\", ";
			profile << "\"ts\" : " << Result.StartTime;
			profile << "},";

			if (m_CurrentSession)
			{
				m_ProfilerStream << profile.str();
				m_ProfilerStream.flush();
			}
		}

		void EndSession()
		{
			EndSession_Internal();
		}

		static Profiler& Get()
		{
			static Profiler s_Profiler;
			return s_Profiler;
		}

	private:
		void WriteHeader()
		{
			m_ProfilerStream << "{ \" otherData\" : { \" version\" : \" GraphX Engine v1.0 \"}, \"traceEvents\" : [";
			m_ProfilerStream.flush();
		}

		void WriteFooter()
		{
			m_ProfilerStream << "{}]}";
			m_ProfilerStream.flush();
		}

		void EndSession_Internal()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_ProfilerStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}
	};

	class ProfilerTimer
	{
		using HighResDuration = std::chrono::duration<double, std::micro>;
	public:
		ProfilerTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimePoint = std::chrono::high_resolution_clock::now();
		}

		~ProfilerTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			std::chrono::high_resolution_clock::time_point endPoint = std::chrono::high_resolution_clock::now();
			double startTime = std::chrono::time_point_cast<HighResDuration>(m_StartTimePoint).time_since_epoch().count();
			double duration = std::chrono::time_point_cast<HighResDuration>(endPoint).time_since_epoch().count() - startTime;

			Profiler::Get().WriteProfile({ m_Name, startTime, duration });

			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::high_resolution_clock::time_point m_StartTimePoint;
		bool m_Stopped;
	};
}

#ifdef GX_PROFILING
	#define GX_PROFILER_BEGIN_SESSION(name, filePath)	::GraphX::Profiler::Get().BeginSession(name, filePath);
	#define GX_PROFILER_END_SESISON()					::GraphX::Profiler::Get().EndSession();
	#define GX_PROFILE_SCOPE(name)						::GraphX::ProfilerTimer timer##__LINE__(name);
	#define GX_PROFILE_FUNCTION()						GX_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define GX_PROFILER_BEGIN_SESSION(name, filePath)	
	#define GX_PROFILER_END_SESISON()					
	#define GX_PROFILE_SCOPE(name)
	#define GX_PROFILE_FUNCTION() 
#endif