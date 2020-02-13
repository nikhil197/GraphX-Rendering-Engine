#include "pch.h"
#include "Clock.h"
#include "Timer.h"

namespace GraphX
{
	std::shared_ptr<Clock> Clock::s_Clock;

	Clock::Clock(ConstructorHelper&& ch)
	{
		m_Timer = new Timer("GraphX Engine");
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
		delete m_Timer;
		m_Timer = timer;
	}

	Clock::~Clock()
	{
		GX_ENGINE_INFO("Destroying Engine Clock");
		delete m_Timer;
	}
}