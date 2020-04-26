#pragma once

namespace GraphX
{
	class Timer;

	class Clock
	{
	private:
		Clock();

	public:
		/* Returns the static instance of the clock */
		inline static std::shared_ptr<Clock>& GetClock() { return s_Clock; }

		/* Initialise the clock */
		static void Init();

		/* Called Every Frame */
		void Tick();

		/* Get the delta time */
		float GetDeltaTime();

		/* Get time since the start of the application */
		float GetEngineTime();

		/* Set a new timer */
		void SetTimer(Timer* timer);

		/* Release resources */
		~Clock();

	private:
		/* Timer for the clock */
		Timer* m_Timer;

		/* Static clock */
		static std::shared_ptr<Clock> s_Clock;
	};
}