#include "pch.h"
#include "Timer.h"

namespace GraphX
{
	Timer::Timer(const char* name, bool accumulate)
		:m_Name(name), m_Accumulate(accumulate), m_StartTimePoint(std::chrono::high_resolution_clock::now()), m_LastUpdateTimePoint(std::chrono::high_resolution_clock::now()), m_Callback(nullptr)
	{}

	Timer::Timer(const char* name, const std::function<void(float)>& callback)
		: m_Name(name), m_Accumulate(false), m_StartTimePoint(std::chrono::high_resolution_clock::now()), m_LastUpdateTimePoint(std::chrono::high_resolution_clock::now()), m_Callback(callback)
	{}

	Timer::Timer(const char* name, std::function<void(float)>&& callback)
		: m_Name(name), m_Accumulate(false), m_StartTimePoint(std::chrono::high_resolution_clock::now()), m_LastUpdateTimePoint(std::chrono::high_resolution_clock::now()), m_Callback(std::move(callback))
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
		if (m_Callback)
		{
			m_Callback(duration.count() * 1000.0f);
		}
		else
		{
			gRunTimeStats.AddCustomStat(m_Name, duration.count() * 1000.0f, m_Accumulate);
		}
	}
}