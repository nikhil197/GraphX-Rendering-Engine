#include "pch.h"
#include "EngineRunTimeStats.h"

/* Global object to store the run time stats */
namespace GraphX
{
	EngineRunTimeStats gRunTimeStats;

	void EngineRunTimeStats::AddCustomStat(const std::string& statName, float sampleValue, bool accumulate)
	{
		if (accumulate)
		{
			m_CustomStats[statName].first += sampleValue;
			m_CustomStats[statName].second += 1;
		}
		else
		{
			m_CustomStats[statName].first = sampleValue;
			m_CustomStats[statName].second = 1;
		}
	}

	void EngineRunTimeStats::ResetCustomStats()
	{
		m_CustomStats.clear();
	}

	void EngineRunTimeStats::OnEngineStartup(float duration)
	{
		m_EngineStartupTime = duration;
	}

	void EngineRunTimeStats::NewFrame(float deltaTime)
	{
		m_FrameCountCurr++;
		m_TimeSincePrevFPS += deltaTime;
		if (m_TimeSincePrevFPS >= 1.0f)
		{
			m_FPS = m_FrameCountCurr;
			m_TimeSincePrevFPS = 0;
			m_FrameCountCurr = 0;
			GX_ENGINE_INFO("Frame Rate: {0} FPS", gRunTimeStats.GetFPS());
		}
	}
}