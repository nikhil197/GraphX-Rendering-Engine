#include "pch.h"
#include "Timer.h"

namespace GraphX
{
	Timer::Timer(const char* name, bool accumulate)
		:m_Name(name), m_Accumulate(accumulate), m_StartTimePoint(std::chrono::high_resolution_clock::now()), m_LastUpdateTimePoint(std::chrono::high_resolution_clock::now())
	{}

	void Timer::Update()
	{
		m_LastUpdateTimePoint = std::chrono::high_resolution_clock::now();
	}

	float Timer::GetDeltaTime() const
	{
		std::chrono::duration<float> duration  = std::chrono::high_resolution_clock::now() - m_LastUpdateTimePoint;
		return duration.count();
	}

	float Timer::GetTime() const
	{
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTimePoint;
		return duration.count();
	}

	Timer::~Timer()
	{
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTimePoint;
		if (m_Accumulate)
		{
			gRunTimeStats.CustomStats[m_Name] = gRunTimeStats.CustomStats[m_Name] + duration.count() * 1000.0f;
		}
		else
		{
			gRunTimeStats.CustomStats[m_Name] = duration.count() * 1000.0f;
		}
	}
}