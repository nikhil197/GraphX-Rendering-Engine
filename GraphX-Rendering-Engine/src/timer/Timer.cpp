#include "Timer.h"

#include <iostream>

namespace engine
{
	Timer::Timer(const std::string& name)
		:m_Name(name), m_StartTimePoint(std::chrono::high_resolution_clock::now()), m_LastUpdateTimePoint(std::chrono::high_resolution_clock::now())
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
		std::cout << "Timer " << m_Name << " took " << duration.count() * 1000.f << " ms." << std::endl;
	}
}