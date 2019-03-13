#include "pch.h"
#include "Clock.h"
#include "Timer.h"

namespace engine
{
	std::shared_ptr<Clock> Clock::s_Clock;

	Clock::Clock(ConstructorHelper&& ch)
	{
		m_Timer = new Timer("GraphX");
	}

	void Clock::Init()
	{
		s_Clock = std::make_shared<Clock>(ConstructorHelper());
	}

	void Clock::Tick()
	{
		m_Timer->Update();
	}

	float Clock::GetDeltaTime()
	{
		return m_Timer->GetDeltaTime();
	}

	float Clock::GetEngineTime()
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