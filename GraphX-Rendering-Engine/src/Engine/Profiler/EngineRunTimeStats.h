#pragma once

namespace GraphX
{
	/* Different run time stats for the engine (All the stats are in milliseconds) */
	class EngineRunTimeStats
	{
	private:
		float m_EngineStartupTime = 0.0f;
		float m_SceneLoadTime = 0.0f;
		int m_FrameCountCurr = 0;
		int m_FPS = 0;
		float m_TimeSincePrevFPS = 0.0f;

		std::unordered_map<std::string, std::pair<float, int>> m_CustomStats;

	public:
		inline float GetEngineStartupTime() const { return m_EngineStartupTime; }

		inline float GetPrevSceneLoadTime() const { return m_SceneLoadTime; }

		inline int GetFPS() const { return m_FPS; }

		const std::unordered_map<std::string, std::pair<float, int>>& GetCustomStats() const { return m_CustomStats; }

	public:
		void AddCustomStat(const std::string& statName, float sampleValue, bool accumulate);

		void ResetCustomStats();

		void OnEngineStartup(float duration);

		void NewFrame(float deltaTime);
	};

	extern EngineRunTimeStats gRunTimeStats;

#define GX_ENGINE_RUN_TIME_STAT_STARTUP(name) Timer PREPROCESSORJOIN(timer, __LINE__)(name, std::function<void(float)>([](float duration) { gRunTimeStats.OnEngineStartup(duration); }));

#define GX_ENGINE_NEW_FRAME gRunTimeStats.RecordNewFrame();

#define GX_ENGINE_RUN_TIME_NEW_FRAME() 

#define GX_ENGINE_RUN_TIME_STAT_AVG(name) Timer PREPROCESSORJOIN(timer, __LINE__)(name, true);
}