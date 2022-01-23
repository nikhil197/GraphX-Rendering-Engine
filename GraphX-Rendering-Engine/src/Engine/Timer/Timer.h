#pragma once

namespace GraphX
{
	/* Different run time stats for the engine (All the stats are in milliseconds) */
	struct EngineRunTimeStats
	{
		float EngineStartupTime;
		float SceneLoadTime;
		std::unordered_map<std::string, float> CustomStats;
	};

	extern EngineRunTimeStats gRunTimeStats;

	class Timer
	{
	public:
		/* Constructor */
		explicit Timer(const char* name, bool accumulate = false);

		/* Update the timer */
		void Update();

		/* Returns the time between two successive updates */
		float GetDeltaTime() const;

		/* Returns the total time elapsed since the start of the Timer */
		float GetTime() const;

		/* Destructor for the timer */
		~Timer();

		/* Returns the name of the timer */
		const char* GetName() const { return m_Name; }

	private:
		/* Name of the timer */
		const char* m_Name;

		/* Whether to accumulate the time values or not */
		bool m_Accumulate;

		/* To store the time point of the last update */
		std::chrono::time_point<std::chrono::steady_clock> m_LastUpdateTimePoint;

		/* To store the time point when the application started */
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
	};
}