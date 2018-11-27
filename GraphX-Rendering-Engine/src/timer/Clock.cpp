#include "Clock.h"
#include "Timer.h"
#include <iostream>

namespace engine
{
	std::shared_ptr<Clock> Clock::s_Clock;

	Clock::Clock()
	{
		m_Timer = new Timer("GraphX");
	}

	void Clock::Init()
	{
		s_Clock = std::make_shared<Clock>();
	}

	void Clock::Tick()
	{
		m_Timer->Update();
	}

	float Clock::GetDeltaTime()
	{
		return m_Timer->GetDeltaTime();
	}

	float Clock::GetTime()
	{
		return m_Timer->GetTime();
	}

	void Clock::SetTimer(Timer* timer)
	{
		m_Timer = timer;
	}

	Clock::~Clock()
	{
		std::cout << "destroy" << std::endl;
		delete m_Timer;
	}
}